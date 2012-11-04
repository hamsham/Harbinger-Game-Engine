/* 
 * File:   serialize.cpp
 * Author: hammy
 * 
 * Created on October 27, 2012, 11:24 PM
 */

#include "../../include/harbinger.h"

namespace harbinger {
namespace serialization {

//function pointers to aid in deserializing scripts.
//CAUTION: All return a script type created using the 'NEW' operator
// CAUTION: All factory arrays follow the order displayed by their corresponding
// enumerations listed in the file "script.h"
//-----------------------------------------------------------------------------
// Script Variable Factory
//-----------------------------------------------------------------------------
#define SCRIPTVARFACTORY( scriptvartype )\
c_scriptVarBase* get_##scriptvartype ( std::ifstream& fin ) {\
	c_##scriptvartype *var( new( std::nothrow ) c_##scriptvartype );\
	if ( var ) {\
		fin >> *var;\
	}\
	return var;\
}

SCRIPTVARFACTORY( scriptInt )
SCRIPTVARFACTORY( scriptUint )
SCRIPTVARFACTORY( scriptFloat )
SCRIPTVARFACTORY( scriptBool )
SCRIPTVARFACTORY( scriptString )
SCRIPTVARFACTORY( scriptVec3d )

typedef c_scriptVarBase* ( *scriptVarFactory )( std::ifstream& fin );
static scriptVarFactory varFactory[] = {
	get_scriptInt,
	get_scriptUint,
	get_scriptFloat,
	get_scriptBool,
	get_scriptString,
	get_scriptVec3d,
};

//-----------------------------------------------------------------------------
// Script Function Factory
//-----------------------------------------------------------------------------
#define SCRIPTFUNCFACTORY( scriptfunctype )\
c_scriptFuncBase* get_##scriptfunctype ( std::ifstream& fin ) {\
	c_##scriptfunctype *var( new( std::nothrow ) c_##scriptfunctype );\
	if ( var ) {\
		fin >> *var;\
	}\
	return var;\
}

SCRIPTFUNCFACTORY( scriptNumEval )
SCRIPTFUNCFACTORY( scriptMiscMath )
SCRIPTFUNCFACTORY( scriptArithmetic )
SCRIPTFUNCFACTORY( scriptTrigonometry )

typedef c_scriptFuncBase* ( *scriptFuncFactory )( std::ifstream& );
static scriptFuncFactory funcFactory[] = {
	get_scriptNumEval,
	get_scriptMiscMath,
	get_scriptArithmetic,
	get_scriptTrigonometry
};


//-----------------------------------------------------------------------------
// serialization extension functions
//-----------------------------------------------------------------------------
bool readHeader( std::ifstream& fin, e_hgeFileType fileType, scriptList& scrList );
bool readFooter( std::ifstream& fin, scriptListSize_t numVars, scriptListSize_t numFuncs );
c_script* loadVar( std::ifstream& fin, scriptVarMap& varMap, int scriptSubType );
c_script* loadFunc( std::ifstream& fin, scriptVarMap& varMap, int scriptSubType );
void unloadData( scriptList& scrList ); //in case of a file read error, clear memory

//-----------------------------------------------------------------------------
// SERIALIZATION - SAVING
// Small enough to not require extra functions
//-----------------------------------------------------------------------------
e_fileStatus saveScripts( const char* fileName, e_hgeFileType fileType, scriptList& inScripts, bool overwriteData ) {
	if ( al_filename_exists( fileName ) && overwriteData == false )
		return FILE_SAVE_OVERWRITE;
	
	std::ofstream fout;
	fout.open( fileName, std::ios_base::binary | std::ios_base::trunc );
	if ( fout.bad() ) {
		return FILE_SAVE_IO_ERROR;
	}
	
	//print a header
	fout
		<< "hge "
		<< HARBINGER_FILE_TYPES[ fileType ]
		<< " "
		<< inScripts.size()
		<< '\n';
	
	//prep a footer
	size_t numVars( 0 );
	size_t numFuncs( 0 );
	
	scriptList::iterator iter;
	c_script* pScript( NULL );
	iter = inScripts.begin();
	
	while ( iter != inScripts.end() ) {
		pScript = *iter;
		fout
			<< pScript->getScriptType() << " "
			<< pScript->getScriptSubType() << " ";
		
		if ( pScript->getScriptType() == SCRIPT_VAR ) {
			//variable addresses will be read back upon deserialization
			fout << (ulong)pScript << " ";
			++numVars;
		}
		else if ( pScript->getScriptType() == SCRIPT_FUNC ) {
			++numFuncs;
		}
		else {
			fout.close();
			if ( overwriteData == true )
				al_remove_filename( fileName );
			return FILE_SAVE_INVALID_DATA;
		}
		fout << *pScript << '\n';
		fout.flush();
		++iter;
	}
	
	//print the footer
	fout << numVars << " " << numFuncs;
	fout.flush();
	
	fout.close();
	return FILE_SAVE_SUCCESS;
}

void unloadData( scriptList& scrList ) {
	scriptList::iterator iter;
	for ( iter != scrList.begin(); iter != scrList.end(); ++iter )
		delete *iter;
}

//-----------------------------------------------------------------------------
// SERIALIZATION - LOADING
// Uses functions to read in the header, footer, variables, and functions
//-----------------------------------------------------------------------------
e_fileStatus loadScripts( const char* fileName, e_hgeFileType fileType, scriptList& outScripts ) {
	
	if ( al_filename_exists( fileName ) == false )
		return FILE_LOAD_INVALID_NAME;
	
	std::ifstream fin( fileName, std::ios_base::binary );
	if ( fin.bad() == true )
		return FILE_LOAD_IO_ERROR;
	
	//read the header
	if ( !readHeader( fin, fileType, outScripts ) ) {
		fin.close();
		return FILE_LOAD_ERROR;
	}
	
	//type verifications
	int scrType( 0 );
	int scrSubType( 0 );
	size_t numVars( 0 );
	size_t numFuncs( 0 );
	
	//create a map/table for all pointers to variables
	scriptVarMap varMap;
	varMap[ 0 ] = NULL; //create a default, invalid, value
	scriptList::iterator iter;
	iter = outScripts.begin(); // memory was preallocated by the "readHeader" function
	
	while ( fin.good() ) {
		
		//read in the data types
		fin >> scrType;
		fin >> scrSubType;
		
		//variable type
		if ( scrType == SCRIPT_VAR ) {
			++numVars;
			*iter = loadVar( fin, varMap, scrSubType );
		}
		//function type
		else if ( scrType == SCRIPT_FUNC ) {
			++numFuncs;
			*iter = loadFunc( fin, varMap, scrSubType );
		}
		else {
			goto invalidFileData;
		}
		
		if ( *iter == NULL ) {
			invalidFileData: //goto used from the "else" statement above
			fin.close();
			unloadData( outScripts );
			return FILE_LOAD_INVALID_DATA;
		}
		++iter;
	}
	
	//read in the footer
	if ( !fin.good()
	|| (varMap.size() != outScripts.size())
	|| !readFooter( fin, numVars, numFuncs ) ) {
		fin.close();
		unloadData( outScripts );
		return FILE_LOAD_ERROR;
	}
	
	fin.close();
	return FILE_LOAD_SUCCESS;
}

//-----------------------------------------------------------------------------
// SERIALIZATION EXTENSION FUNCTIONS
//-----------------------------------------------------------------------------
bool readHeader( std::ifstream& fin, e_hgeFileType fileType, scriptList& scrList ) {
	std::string inData;
	scriptList::size_type numItems( 0 );
	
	fin >> inData;
	if ( inData != "hge" )
		return false;
		
	fin >> inData;
	if ( inData != HARBINGER_FILE_TYPES[ fileType ] )
		return false;
	
	fin >> numItems;
	if ( numItems < scrList.max_size() )
		scrList.resize( numItems, NULL );
	else
		return false;
	
	return true;
}

bool readFooter( std::ifstream& fin, size_t numVars, size_t numFuncs ) {
	size_t varVerification( 0 );
	size_t funcVerification( 0 );
	fin >> varVerification >> funcVerification;

	if ( (numVars != varVerification) || (numFuncs != funcVerification) ) {
		return false;
	}
}
	
c_script* loadVar( std::ifstream& fin, scriptVarMap& varMap, int varType ) {
	if ( varType < 0 || varType >= SCRIPT_VAR_MAX )
		return NULL;
	
	ulong varRefNum( 0 ); //reference variable for script Vars
	fin >> varRefNum;
	
	c_scriptVarBase* pVar( NULL );
	pVar = varFactory[ varType ]( fin ); //function pointer call
	
	varMap[ varRefNum ] = pVar;
	return pVar;
}

c_script* loadFunc( std::ifstream& fin, scriptVarMap& varMap, int scrSubType ) {
	if ( scrSubType < 0 || scrSubType >= SCRIPT_FUNC_MAX )
		return NULL;
	
	ulong varRefNum( 0 ); //reference variable for script Vars
	c_scriptFuncBase* pFunc(
		funcFactory[ scrSubType ]( fin ) //more function pointers
	);
	if ( pFunc == NULL ) return NULL;

	//restore the function's variables (the static casts are safe due to the function factory)
	if ( scrSubType == SCRIPT_FUNC_NUM_EVAL ) {
		c_scriptNumEval* numEval = dynamic_cast< c_scriptNumEval* >( pFunc );
		fin >> varRefNum;
		numEval->setVarToEvaluate(
			reinterpret_cast<c_scriptNum*>( varMap[ varRefNum ] )
		);
		fin >> varRefNum;
		numEval->setVarToCompare(
			reinterpret_cast<c_scriptNum*>( varMap[ varRefNum ] )
		);
	}
	
	else if ( scrSubType == SCRIPT_FUNC_NUM_MISC
	|| SCRIPT_FUNC_NUM_ARITH
	|| SCRIPT_FUNC_NUM_TRIG ) {
		c_scriptNumeric* numEval = dynamic_cast< c_scriptNumeric* >( pFunc );
		fin >> varRefNum;
		numEval->setVarToEvaluate(
			reinterpret_cast<c_scriptNum*>( varMap[ varRefNum ] )
		);
		fin >> varRefNum;
		numEval->setVarToCompare(
			reinterpret_cast<c_scriptNum*>( varMap[ varRefNum ] )
		);
	}
	
	else {
		//kill the loading functions if an invalid type is found
		return NULL;
	}
	
	return pFunc;
}

} // end serialization namespace

//-----------------------------------------------------------------------------
// iostream operators - Definitions
//-----------------------------------------------------------------------------
/* A note about the stream operators:
* The script type and sub-type will be printed when being sent to an ostream
* but they are not read back in by an istream.
* This is because the script type and sub-type must be determined before
* reading in any object data. This makes it much easier to determine what
* type of polymorphic object should be loaded when saving to/loading from files
*/
std::ostream& operator << ( std::ostream& sout, const c_script& scr ) {
	sout
		<< scr.getScriptType() << " "
		<< scr.getScriptSubType() << " "
		<< scr.name;
	return sout;
}

std::istream& operator >> ( std::istream& stin, c_script& scr ) {
	stin
		>> scr.name;
	return stin;
}

//-----------------------------------------------------------------------------
// Evaluation Functions
//-----------------------------------------------------------------------------
std::ostream& operator << ( std::ostream& sout, const c_scriptEvaluation& scr ) {
	sout
		<< scr.getScriptType() << " "
		<< scr.getScriptSubType() << " "
		<< scr.name << " "
		<< scr.evalType << " "
		<< scr.returnVal << " "
		<< (ulong)scr.evalVar << " "
		<< (ulong)scr.compVar;
		
	return sout;
}

std::istream& operator >> ( std::istream& stin, c_scriptEvaluation& scr ) {
	stin
		>> scr.name
		>> scr.evalType
		>> scr.returnVal;
		//varToEvaluate and varToReference are neither read in or saved
		//this is so that there can be a "master" serialization function
		//to manage the saving and loading of pointers
		
	return stin;
}

//-----------------------------------------------------------------------------
// Numerical Functions (same as Evaluations)
//-----------------------------------------------------------------------------
std::ostream& operator << ( std::ostream& sout, const c_scriptNumeric& scr ) {
	sout
		<< scr.getScriptType() << " "
		<< scr.getScriptSubType() << " "
		<< scr.name << " "
		<< scr.evalType << " "
		<< scr.returnVal << " "
		<< (ulong)scr.evalVar << " "
		<< (ulong)scr.compVar;
		
	return sout;
}

std::istream& operator >> ( std::istream& stin, c_scriptNumeric& scr ) {
	stin
		>> scr.name
		>> scr.evalType
		>> scr.returnVal;
		//varToEvaluate and varToReference are neither read in or saved
		//this is so that there can be a "master" serialization function
		//to manage the saving and loading of pointers
		
	return stin;
}

//-----------------------------------------------------------------------------
// String Variables
//-----------------------------------------------------------------------------
std::ostream& operator << ( std::ostream& sout, const c_scriptString& scr ) {
	sout
		<< scr.getScriptType() << " "
		<< scr.getScriptSubType() << " "
		<< scr.name
		<< scr.variable << '\0';
		//very important NULL-termination. Necessary for reading data back from files
	return sout;
}

std::istream& operator >> ( std::istream& stin, c_scriptString& scr ) {
	stin
		>> scr.name;
	
	std::streampos string_begin;
	std::string::size_type num_chars( 0 );
	string_begin = stin.tellg();
	char null_term( 1 );
	
	while ( null_term != 0 ) {
		null_term = stin.get();
	}
	
	//I have tested this extensively using G++ & MSVC++.
	//It NULL-terminates and gives the correct string size.
	num_chars = (stin.tellg() - string_begin) - 1;
	scr.variable.resize( num_chars );
	stin.read( (char*)scr.variable.data(), num_chars );
	scr.variable[ num_chars ] = '\0';
	
	return stin;
}

} // end harbinger namespace