/* 
 * File:   serialize.cpp
 * Author: hammy
 * 
 * Created on October 27, 2012, 11:24 PM
 */

#include "../../include/harbinger.h"
namespace harbinger {

//-----------------------------------------------------------------------------
// Script Factories
//-----------------------------------------------------------------------------
#define SCRIPTFACTORY( scriptBaseType, scriptvartype )\
HL_INLINE c_##scriptBaseType* get_##scriptvartype () {\
		return new( std::nothrow ) c_##scriptvartype();\
}

//CAUTION: All factories return a script type created using the 'NEW' operator
// CAUTION: All factory arrays follow the order displayed by their corresponding
// enumerations listed in the file "script.h"
#define SCRIPTVARFACTORY( x ) SCRIPTFACTORY( scriptVarBase, x )
#define SCRIPTFUNCFACTORY( x ) SCRIPTFACTORY( scriptFuncBase, x )

// Variable Factories
SCRIPTVARFACTORY( scriptInt )
SCRIPTVARFACTORY( scriptUint )
SCRIPTVARFACTORY( scriptFloat )
SCRIPTVARFACTORY( scriptBool )
SCRIPTVARFACTORY( scriptString )
SCRIPTVARFACTORY( scriptVec3 )

// Function Factories
SCRIPTFUNCFACTORY( scriptNumEval )
SCRIPTFUNCFACTORY( scriptMiscMath )
SCRIPTFUNCFACTORY( scriptArithmetic )
SCRIPTFUNCFACTORY( scriptTrigonometry )

//Function pointers to aid in creating script objects.
// Variable Factories
typedef c_scriptVarBase* ( *scriptVarFactory )();
static scriptVarFactory varFactory[] = {
	get_scriptInt,
	get_scriptUint,
	get_scriptFloat,
	get_scriptBool,
	get_scriptString,
	get_scriptVec3,
};

// Function Factories
typedef c_scriptFuncBase* ( *scriptFuncFactory )();
static scriptFuncFactory funcFactory[] = {
	get_scriptNumEval,
	get_scriptMiscMath,
	get_scriptArithmetic,
	get_scriptTrigonometry
};
//-----------------------------------------------------------------------------
// Utility functions
//-----------------------------------------------------------------------------
e_hgeFileType c_serialize::getFileType( const char* fileName ) const {
	std::string fileExt( fileName );
	std::size_t pos( fileExt.find_last_of('.', fileExt.size()-4) );
	
	if ( pos == std::string::npos ) {
		return HGE_SCRIPT_FILE_INVALID;
	}
	
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
c_serialize::e_fileStatus c_serialize::saveScripts( const char* fileName, const scriptList_t& inScripts, bool overwriteData ) {
	//read in the file extension
	e_hgeFileType fileType = getFileType( fileName );
	
	if ( fileType == HGE_SCRIPT_FILE_INVALID )
		return FILE_SAVE_INVALID_NAME;
	if ( al_filename_exists( fileName ) && overwriteData == false )
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
	size_t numVars( 0 ); // numVars & numFuncs count the amount of scripts that have been processed
	size_t numFuncs( 0 );// not the amount of scripts that are in "varMap" or "funcMap"
	
	const c_script* pScript( NULL );
	scriptList_t::const_iterator iter;
	iter = inScripts.begin();
	
	while ( iter != inScripts.end() ) {
		pScript = (*iter);
		
		if ( (*iter)->getScriptType() == SCRIPT_VAR ) {
			++numVars;
		}
		else if ( (*iter)->getScriptType() == SCRIPT_FUNC ) {
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
c_serialize::e_fileStatus c_serialize::loadScripts( const char* fileName, scriptList_t& outScripts ) {
	if ( outScripts.size() != 0 )
		return FILE_LOAD_OVERWRITE;
	
	//open the file and prepare the stream flags
	std::ifstream fileIO( fileName, std::ios_base::in | std::ios_base::binary );
	if ( fileIO.good() == false || readHeader( fileIO, outScripts ) == false ) {
		fileIO.close();
		return FILE_LOAD_ERROR;
	}
	fileIO.flags( std::ios::skipws | std::ios::fixed );
	
	//type verifications
	scriptMap_t scrMap;
	c_script* pScript( NULL );
	int scrType( 0 );
	int scrSubType( 0 );
	size_t numVars( 0 ); // numVars & numFuncs count the amount of scripts that have been processed
	size_t numFuncs( 0 );// not the amount of scripts that are in "varMap" or "funcMap"
	
	scriptList_t::iterator iter;
	iter = outScripts.begin(); // memory was preallocated by the "readHeader" function
	
	while ( fileIO.good() ) {		
		if ( iter == outScripts.end() )
			break;
		
		//read in the data types
		fileIO >> scrType >> scrSubType;
		
		//variable type
		if ( scrType == SCRIPT_VAR ) {
			HGE_ASSERT ( (scrSubType > SCRIPT_INVALID && scrSubType < SCRIPT_VAR_MAX) );
			pScript = varFactory[ scrSubType ](); //function pointer call
			++numVars;
		}
		//function type
		else if ( scrType == SCRIPT_FUNC ) {
			HGE_ASSERT ( (scrSubType > SCRIPT_INVALID && scrSubType < SCRIPT_FUNC_MAX) );
			pScript = funcFactory[ scrSubType ](); //function pointer call
			++numFuncs;
		}
		else { // invalid file data
			fileIO.close();
			unloadData( outScripts );
			return FILE_LOAD_INVALID_DATA;
		}
		
		//retrieve the script type
		if ( pScript != NULL ) {
			pScript->read( fileIO, scrMap );
		}
		else {
			fileIO.close();
			unloadData( outScripts );
			return FILE_LOAD_IO_ERROR;
		}
		*iter = pScript;
		++iter;
	}
	
	//read in the footer
	if ( !fileIO.good()
	|| !readFooter( fileIO, outScripts, numVars, numFuncs) ) {
		fileIO.close();
		unloadData(outScripts );
		return FILE_LOAD_ERROR;
	}
	
	fileIO.close();
	return FILE_LOAD_SUCCESS;
}

//-----------------------------------------------------------------------------
// Clear memory in case of error
//-----------------------------------------------------------------------------
void c_serialize::unloadData( scriptList_t& inScripts ) {
	scriptList_t::iterator iter;
	
	for ( iter = inScripts.begin(); iter != inScripts.end(); ++iter ) {
		delete *iter;
	}
	inScripts.clear();
}

//-----------------------------------------------------------------------------
// SERIALIZATION EXTENSION FUNCTIONS
//-----------------------------------------------------------------------------
bool c_serialize::readHeader( std::ifstream& fileIO, scriptList_t& scrList ) {
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
	scrList.resize( numItems, NULL );
	return true;
}

bool c_serialize::readFooter( std::ifstream& fileIO, scriptList_t& scrList, size_t numVars, size_t numFuncs ) {
	size_t varVerification( 0 );
	size_t funcVerification( 0 );
	fileIO >> varVerification >> funcVerification;

	if ( (numVars != varVerification) || (numFuncs != funcVerification) )
		return false;
	if ( (numVars + numFuncs) != scrList.size() )
		return false;
	
	return true;
}

} // end harbinger namespace