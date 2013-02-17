/* 
 * File:   serialize.cpp
 * Author: hammy
 * 
 * Created on October 27, 2012, 11:24 PM
 */

#include <stdexcept>
#include <string>
#include <fstream>

#include "types.h"
#include "resource.h"

#include "scripting/script_base.h"
#include "scripting/script_variables.h"
#include "scripting/script_functions.h"
#include "scripting/script_manager.h"
#include "scripting/script_serializer.h"

//-----------------------------------------------------------------------------
// Utility functions
//-----------------------------------------------------------------------------
e_hgeFileType c_serializer::getFileType( cstr fileName ) const {
	std::string fileExt( fileName );
	// attempt to find the type of file based on the filename extension
	try {
		// "+1" added in order to remove the leading period
		fileExt = fileExt.substr( fileExt.find_last_of('.', fileExt.size()-4) +1 );
	}
	catch ( const std::out_of_range& oor ) {
		return HGE_SCRIPT_FILE_INVALID;
	}
	
	fileExt[0] = std::tolower( fileExt[0] );
	fileExt[1] = std::tolower( fileExt[1] );
	fileExt[2] = std::tolower( fileExt[2] );
	
	if ( fileExt == HARBINGER_FILE_TYPE[ 0 ] ) {
		return HGE_SCRIPT_RAW_DATA;
	}
	
	if ( fileExt == HARBINGER_FILE_TYPE[ 1 ] ) {
		return HGE_SCRIPT_EDITOR_DATA;
	}
	
	return HGE_SCRIPT_FILE_INVALID;
}

//-----------------------------------------------------------------------------
// SERIALIZATION - SAVING
// Small enough to not require extra functions
//-----------------------------------------------------------------------------
c_serializer::e_fileStatus c_serializer::saveScripts( cstr fileName, const scriptList_t& inScripts, bool overwriteData ) {
	//read in the file extension
	e_hgeFileType fileType = getFileType( fileName );
	if ( fileType == HGE_SCRIPT_FILE_INVALID )
		return FILE_SAVE_INVALID_NAME;
	
	if ( c_resource::fileExists( fileName ) && overwriteData == false )
		return FILE_SAVE_OVERWRITE;
	
	//open the file and prepare the stream flags
	std::ofstream fileIO( fileName, std::ios_base::trunc | std::ios_base::binary );
	if ( fileIO.bad() ) {
		return FILE_SAVE_IO_ERROR;
	}
	fileIO.flags( std::ios_base::fixed );
	
	//print a header
	fileIO
		<< HARBINGER_FILE_TYPE[ fileType ] << " "
		<< fileType << " "
		<< inScripts.size() << '\n';
	
	//prep a footer
	hgeSize_t numVars( 0 ); // numVars & numFuncs count the amount of scripts that have been processed
	hgeSize_t numFuncs( 0 );// not the amount of scripts that are in "varMap" or "funcMap"
	
	const c_script* pScript( HGE_NULL );
	scriptList_t::const_iterator iter;
	iter = inScripts.begin();
	
	while ( iter != inScripts.end() ) {
		pScript = (*iter);
		
		if ( pScript->getScriptType() == SCRIPT_VAR ) {
			++numVars;
		}
		else if ( pScript->getScriptType() == SCRIPT_FUNC ) {
			++numFuncs;
		}
		else {
			//an invalid file type was found while saving. write the footer and quit
			fileIO << numVars << " " << numFuncs;
			fileIO.close();
			return FILE_SAVE_INVALID_DATA;
		}
		
		//write the script object to the file
		fileIO
			<< pScript->getScriptType() << " "
			<< pScript->getScriptSubType() << " ";
		pScript->write( fileIO );
		
		// save the editor position if requested
#ifdef HGE_EDITOR
		if ( fileType == HGE_SCRIPT_EDITOR_DATA )
			fileIO << pScript->editorPos[0] << " " << pScript->editorPos[1];
#endif /* HGE_EDITOR */
		
		fileIO << '\n';
		++iter;
	} // scriptList iteration loop
	
	//print the footer
	fileIO << numVars << " " << numFuncs;
	fileIO.close();
	return FILE_SAVE_SUCCESS;
}

//-----------------------------------------------------------------------------
// SERIALIZATION - LOADING
// Uses functions to read in the header, footer, variables, and functions
//-----------------------------------------------------------------------------
c_serializer::e_fileStatus c_serializer::loadScripts( cstr fileName, scriptList_t& outScripts ) {
	if ( outScripts.size() != 0 )
		return FILE_LOAD_OVERWRITE;
	
	//open the file and prepare the stream flags
	std::ifstream fileIO( fileName, std::ios_base::in | std::ios_base::binary );
	if ( !fileIO.good() )
		return FILE_LOAD_ERROR;
	
	// read the file type and header
	e_hgeFileType fileType = getFileType( fileName );
	if ( fileType == HGE_SCRIPT_FILE_INVALID || readHeader( fileIO, outScripts ) == false ) {
		fileIO.close();
		return FILE_LOAD_INVALID_TYPE;
	}
	fileIO.flags( std::ios::skipws | std::ios::fixed );
	
	//type verifications
	scriptMap_t scrMap;
	c_script* pScript( HGE_NULL );
	int scrType( 0 );
	int scrSubType( 0 );
	hgeSize_t numVars( 0 ); // numVars & numFuncs count the amount of scripts that have been processed
	hgeSize_t numFuncs( 0 );// not the amount of scripts that are in "varMap" or "funcMap"
	
	scriptList_t::iterator iter;
	iter = outScripts.begin(); // memory was preallocated by the "readHeader" function
	
	while ( fileIO.good() ) {		
		if ( iter == outScripts.end() )
			break;
		
		//read in the data types
		fileIO >> scrType >> scrSubType;
		
		//variable type
		if ( scrType == SCRIPT_VAR ) {
			pScript = c_scriptManager::getVarInstance( scrType );
			++numVars;
		}
		//function type
		else if ( scrType == SCRIPT_FUNC ) {
			pScript = c_scriptManager::getFuncInstance( scrType );
			++numFuncs;
		}
		else { // invalid file data
			fileIO.close();
			unloadData( outScripts );
			return FILE_LOAD_INVALID_DATA;
		}
		
		//retrieve the script type
		if ( pScript != HGE_NULL ) {
			pScript->read( fileIO, scrMap );
		}
		else {
			fileIO.close();
			unloadData( outScripts );
			return FILE_LOAD_IO_ERROR;
		}
		// script successfully loaded. Add it to the list
#ifdef HGE_EDITOR
		if ( fileType == HGE_SCRIPT_EDITOR_DATA )
			fileIO >> pScript->editorPos[0] >> pScript->editorPos[1];
#endif
		*iter = pScript;
		++iter;
	}
	
	//read in the footer
	if ( !fileIO.good()
	|| !readFooter( fileIO, outScripts, numVars, numFuncs) ) {
		fileIO.close();
		unloadData( outScripts );
		return FILE_LOAD_ERROR;
	}
	
	fileIO.close();
	return FILE_LOAD_SUCCESS;
}

//-----------------------------------------------------------------------------
// Clear memory in case of error
//-----------------------------------------------------------------------------
void c_serializer::unloadData( scriptList_t& inScripts ) {
	scriptList_t::iterator iter;
	
	for ( iter = inScripts.begin(); iter != inScripts.end(); ++iter ) {
		c_scriptManager::killInstance( *iter );
	}
	inScripts.clear();
}

//-----------------------------------------------------------------------------
// SERIALIZATION EXTENSION FUNCTIONS
//-----------------------------------------------------------------------------
bool c_serializer::readHeader( std::ifstream& fileIO, scriptList_t& scrList ) {
	int fileType;
	std::string inDataType;
	scriptList_t::size_type numItems( 0 );
	
	//check if the file is supported
	fileIO >> inDataType;
	if ( inDataType != HARBINGER_FILE_TYPE[ 0 ]
	&& inDataType != HARBINGER_FILE_TYPE[ 1 ] ) {
		return false;
	}
		
	fileIO >> fileType;
	if ( fileType != HGE_SCRIPT_FILE_INVALID ) {
		return false;
	}
	
	/* TODO:
	 * Confirm with the previous entry that the file types match
	 */
	
	fileIO >> numItems;
	if ( numItems >= scrList.max_size() ) {
		return false;
	}
	scrList.resize( numItems, HGE_NULL );
	return true;
}

bool c_serializer::readFooter( std::ifstream& fileIO, scriptList_t& scrList, hgeSize_t numVars, hgeSize_t numFuncs ) {
	hgeSize_t varVerification( 0 );
	hgeSize_t funcVerification( 0 );
	fileIO >> varVerification >> funcVerification;

	if ( (numVars != varVerification) || (numFuncs != funcVerification) )
		return false;
	if ( (numVars + numFuncs) != scrList.size() )
		return false;
	
	return true;
}
