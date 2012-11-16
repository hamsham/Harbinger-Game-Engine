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
c_##scriptBaseType* get_##scriptvartype () {\
	c_##scriptvartype *var( new( std::nothrow ) c_##scriptvartype );\
	return var;\
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
SCRIPTVARFACTORY( scriptVec3d )

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
	get_scriptVec3d,
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
	
	if ( (fileExt[ ++pos ] == 'h' || fileExt[ pos ] == 'H')
	&& (fileExt[ ++pos ] == 's' || fileExt[ pos ] == 'S')
	&& (fileExt[ ++pos ] == 'd' || fileExt[ pos ] == 'D') ) {
		return HGE_SCRIPT_FILE_DATA;
	}
	
	return HGE_SCRIPT_FILE_INVALID;
}

void c_serialize::closeStream() {
	fileIO.close();
	fileIO.clear();
}

//-----------------------------------------------------------------------------
// SERIALIZATION - SAVING
// Small enough to not require extra functions
//-----------------------------------------------------------------------------
c_serialize::e_fileStatus c_serialize::saveScripts( const char* fileName, const scriptList& inScripts, bool overwriteData ) {
	//read in the file extension
	if ( getFileType( fileName ) == HGE_SCRIPT_FILE_INVALID )
		return FILE_SAVE_INVALID_NAME;
	
	fileIO.open( fileName, std::ios_base::app );
	if ( fileIO.bad() ) {
		return FILE_SAVE_IO_ERROR;
	}
	if ( fileIO.good() && overwriteData == false )
		return FILE_SAVE_OVERWRITE;
	
	closeStream();
	fileIO.flags( std::ios_base::fixed );
	fileIO.open( fileName, std::ios_base::out | std::ios_base::binary | std::ios_base::trunc );
	
	//print a header
	fileIO
		<< HARBINGER_FILE_TYPE << " "
		<< HGE_SCRIPT_FILE_DATA << " "
		<< inScripts.size() << '\n';
	
	//prep a footer
	size_t numVars( 0 ); // numVars & numFuncs count the amount of scripts that have been processed
	size_t numFuncs( 0 );// not the amount of scripts that are in "varMap" or "funcMap"
	
	c_scriptVarBase* pVar( NULL );
	c_scriptFuncBase* pFunc( NULL );
	scriptList::iterator iter;
	iter = const_cast< scriptList* >( &inScripts)->begin();
	
	while ( iter != inScripts.end() ) {
		fileIO
			<< (*iter)->getScriptType() << " "
			<< (*iter)->getScriptSubType() << " ";
			//<< (*iter) << " "; //use the heap pointer as a UUID
		//"-sizeof(void*)" is used due to a strange incrementation of the UUID/pointer when saved to the file
		
		if ( (*iter)->getScriptType() == SCRIPT_VAR ) {
			pVar = reinterpret_cast< c_scriptVarBase* >( *iter );
			fileIO << pVar << " ";
			writeVar( pVar, pVar->getScriptSubType() );
			++numVars;
		}
		else if ( (*iter)->getScriptType() == SCRIPT_FUNC ) {
			pFunc = reinterpret_cast< c_scriptFuncBase* >( *iter );
			fileIO << pFunc << " ";
			writeFunc( pFunc, pFunc->getScriptSubType() );
			++numFuncs;
		}
		else {
			//an invalid file type was found while saving. write the footer and quit
			fileIO << numVars << " " << numFuncs;
			closeStream();
			return FILE_SAVE_INVALID_DATA;
		}
		
		fileIO << '\n';
		fileIO.flush(); //not trying to overflow the buffer
		++iter;
	}
	
	//print the footer
	fileIO << numVars << " " << numFuncs;
	closeStream();
	return FILE_SAVE_SUCCESS;
}

//-----------------------------------------------------------------------------
// SERIALIZATION - LOADING
// Uses functions to read in the header, footer, variables, and functions
//-----------------------------------------------------------------------------
c_serialize::e_fileStatus c_serialize::loadScripts( const char* fileName, scriptList& outScripts ) {
	if ( outScripts.size() != 0 )
		return FILE_LOAD_OVERWRITE;
	
	fileIO.open( fileName, std::ios_base::in | std::ios_base::binary );
	fileIO.flags( std::ios::skipws | std::ios::fixed );
	
	if ( fileIO.good() == false || readHeader( outScripts ) == false ) {
		closeStream();
		return FILE_LOAD_ERROR;
	}
	
	//type verifications
	c_scriptVarBase* pVar( NULL );
	c_scriptFuncBase* pFunc( NULL );
	int scrType( 0 );
	int scrSubType( 0 );
	size_t numVars( 0 ); // numVars & numFuncs count the amount of scripts that have been processed
	size_t numFuncs( 0 );// not the amount of scripts that are in "varMap" or "funcMap"
	ulong scriptUUID( 0 );
	
	scriptList::iterator iter;
	iter = outScripts.begin(); // memory was preallocated by the "readHeader" function
	
	while ( fileIO.good() ) {
		
		if ( iter == outScripts.end() )
			break;
		
		//read in the data types
		fileIO >> scrType;
		fileIO >> scrSubType;
		fileIO >> scriptUUID;
		
		//variable type
		if ( scrType == SCRIPT_VAR ) {
			HGE_ASSERT ( (scrSubType > SCRIPT_INVALID && scrSubType < SCRIPT_VAR_MAX) );
			++numVars;
			pVar = varFactory[ scrSubType ](); //function pointer call
			readVar( pVar, scrSubType );
			varMap[ scriptUUID ] = pVar;
			*iter = pVar;
		}
		//function type
		else if ( scrType == SCRIPT_FUNC ) {
			HGE_ASSERT ( (scrSubType > SCRIPT_INVALID && scrSubType < SCRIPT_FUNC_MAX) );
			++numFuncs;
			pFunc = funcFactory[ scrSubType ](); //function pointer call
			readFunc( pFunc, scrSubType );
			funcMap[ scriptUUID ] = pFunc;
			*iter = pFunc;
		}
		else { // invalid file data
			goto scriptLoadError;
		}
		if ( *iter == NULL ){
			scriptLoadError:
			closeStream();
			unloadData( outScripts );
			return FILE_LOAD_INVALID_DATA;
		}
		++iter;
	}
	
	//read in the footer
	if ( !fileIO.good()
	|| !readFooter( outScripts, numVars, numFuncs) ) {
		closeStream();
		unloadData(outScripts );
		return FILE_LOAD_ERROR;
	}
	
	varMap.clear();
	funcMap.clear();
	closeStream();
	return FILE_LOAD_SUCCESS;
}

//-----------------------------------------------------------------------------
// Clear memory in case of error
//-----------------------------------------------------------------------------
void c_serialize::unloadData( scriptList& inScripts ) {
	varMap.clear();
	funcMap.clear();
	scriptList::iterator iter;
	
	for ( iter = inScripts.begin(); iter != inScripts.end(); ++iter ) {
		delete *iter;
	}
	inScripts.clear();
}

//-----------------------------------------------------------------------------
// SERIALIZATION EXTENSION FUNCTIONS
//-----------------------------------------------------------------------------
bool c_serialize::readHeader( scriptList& scrList ) {
	int fileType;
	std::string inData;
	scriptList::size_type numItems( 0 );
	
	fileIO >> inData;
	if ( inData != HARBINGER_FILE_TYPE ) {
		return false;
	}
		
	fileIO >> fileType;
	if ( fileType != HGE_SCRIPT_FILE_DATA ) {
		return false;
	}
	
	fileIO >> numItems;
	if ( numItems >= scrList.max_size() ) {
		return false;
	}
	scrList.resize( numItems, NULL );
	return true;
}

bool c_serialize::readFooter( scriptList& scrList, size_t numVars, size_t numFuncs ) {
	size_t varVerification( 0 );
	size_t funcVerification( 0 );
	fileIO >> varVerification >> funcVerification;

	if ( (numVars != varVerification) || (numFuncs != funcVerification) )
		return false;
	if ( (numVars + numFuncs) != scrList.size() )
		return false;
	
	return true;
}

//-----------------------------------------------------------------------------
//	Input Functions
//-----------------------------------------------------------------------------
void c_serialize::readVar( c_scriptVarBase* var, int varSubType ) {
	if ( varSubType == SCRIPT_VAR_INT ) {
		fileIO >> dynamic_cast< c_scriptInt* >( var )->variable;
		return;
	}
	else if ( varSubType == SCRIPT_VAR_UINT ) {
		fileIO >> dynamic_cast< c_scriptUint* >( var )->variable;
		return;
	}
	else if ( varSubType == SCRIPT_VAR_FLOAT ) {
		fileIO >> dynamic_cast< c_scriptFloat* >( var )->variable;
		return;
	}
	else if ( varSubType == SCRIPT_VAR_BOOL ) {
		fileIO >> dynamic_cast< c_scriptBool* >( var )->variable;
		return;
	}
	else if ( varSubType == SCRIPT_VAR_STRING ) {
		std::string::size_type strSize( 0 );
		std::string::size_type iter( 0 );
		std::string& str = dynamic_cast< c_scriptString* >( var )->variable;
		fileIO >> strSize;
		if ( strSize == 0 ) return;
		fileIO.get(); // discard the next whitespace before reading in the string
		str.resize( strSize );
		while ( iter <= strSize ) {
			str[ iter ] = fileIO.get();
			++iter;
		}
		return;
	}
	else if ( varSubType == SCRIPT_VAR_VEC3 ) {
		fileIO >> dynamic_cast< c_scriptBool* >( var )->variable;
		return;
	}
}

void c_serialize::readFunc( c_scriptFuncBase* func, int funcSubType ) {
	ulong uuid( 0 );
	
	if ( funcSubType == SCRIPT_FUNC_NUM_EVAL ) {
		c_scriptEvaluation* eval = dynamic_cast< c_scriptEvaluation* >( func );
		
		fileIO >> eval->evalType >> eval->returnVal.variable;
		fileIO >> uuid; eval->evalVar = varMap[ uuid ];
		fileIO >> uuid; eval->compVar = varMap[ uuid ];
		fileIO >> uuid; eval->nextFunc = funcMap[ uuid ];
		return;
	}
	else if ( funcSubType == SCRIPT_FUNC_NUM_MISC
	||  funcSubType == SCRIPT_FUNC_NUM_TRIG
	||  funcSubType == SCRIPT_FUNC_NUM_ARITH) {
		c_scriptNumeric* numeric = dynamic_cast< c_scriptNumeric* >( func );
		
		fileIO >> numeric->evalType >> numeric->returnVal.variable;
		fileIO >> uuid;
		numeric->evalVar = dynamic_cast< const c_scriptNum* >(varMap[ uuid ]);
		fileIO >> uuid;
		numeric->compVar = dynamic_cast< const c_scriptNum* >(varMap[ uuid ]);
		fileIO >> uuid; numeric->nextFunc = funcMap[ uuid ];
		return;
	}
}

//-----------------------------------------------------------------------------
//	Output Functions
//-----------------------------------------------------------------------------
void c_serialize::writeVar( c_scriptVarBase* var, int varSubType ) {
	if ( varSubType == SCRIPT_VAR_INT ) {
		fileIO << dynamic_cast< c_scriptInt* >( var )->variable;
		return;
	}
	else if ( varSubType == SCRIPT_VAR_UINT ) {
		fileIO << dynamic_cast< c_scriptUint* >( var )->variable;
		return;
	}
	else if ( varSubType == SCRIPT_VAR_FLOAT ) {
		fileIO << dynamic_cast< c_scriptFloat* >( var )->variable;
		return;
	}
	else if ( varSubType == SCRIPT_VAR_BOOL ) {
		fileIO << dynamic_cast< c_scriptBool* >( var )->variable;
		return;
	}
	else if ( varSubType == SCRIPT_VAR_STRING ) {
		std::string& str = dynamic_cast< c_scriptString* >( var )->variable;
		fileIO << str.size() << " " << str.c_str();
		return;
	}
	else if ( varSubType == SCRIPT_VAR_VEC3 ) {
		fileIO << dynamic_cast< c_scriptVec3d* >( var )->variable;
		return;
	}
}

void c_serialize::writeFunc( c_scriptFuncBase* func, int funcSubType ) {
	if ( funcSubType == SCRIPT_FUNC_NUM_EVAL ) {
		c_scriptEvaluation* eval;
		eval = dynamic_cast< c_scriptEvaluation* >( func );
		
		fileIO
			<< eval->evalType << " "
			<< eval->returnVal.variable << " "
			<< eval->evalVar << " "
			<< eval->compVar << " "
			<< eval->nextFunc;
		return;
	}
	else if ( funcSubType == SCRIPT_FUNC_NUM_MISC
	||  funcSubType == SCRIPT_FUNC_NUM_TRIG
	||  funcSubType == SCRIPT_FUNC_NUM_ARITH) {
		c_scriptNumeric* numeric;
		numeric = dynamic_cast< c_scriptNumeric* >( func );
		
		fileIO
			<< numeric->evalType << " "
			<< numeric->returnVal.variable << " "
			<< numeric->evalVar << " "
			<< numeric->compVar << " "
			<< numeric->nextFunc;
		return;
	}
}

} // end harbinger namespace