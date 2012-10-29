/* 
 * File:   serialize.cpp
 * Author: hammy
 * 
 * Created on October 27, 2012, 11:24 PM
 */

#include "../../include/harbinger.h"

namespace harbinger {
namespace serialization {

//------------------------------------------------------------------------
// SERIALIZATION - SAVING
//------------------------------------------------------------------------
e_fileStatus saveScripts(
	const char* fileName,
	e_hgeFileType fileType,
	scriptList& inScripts,
	bool overwriteData
) {
	if ( al_filename_exists( fileName ) && overwriteData == false )
		return (e_fileStatus)(FILE_SAVE_ERROR | FILE_SAVE_OVERWRITE);
	
	std::ofstream fout;
	fout.open( fileName, std::ios_base::binary | std::ios_base::trunc );
	scriptList::iterator iter;
	c_script* pScript( NULL );
	iter = inScripts.begin();
	
	while ( iter != inScripts.end() ) {
		pScript = *iter;
		if ( pScript->getScriptType() == SCRIPT_VAR ) {
			fout << (void*)pScript << " "; // addresses will be read back upon deserialization
		}
		fout << *pScript << std::endl;
		++iter;
	}
	
	fout.close();
	return FILE_SAVE_SUCCESS;
}

//------------------------------------------------------------------------
// SERIALIZATION - LOADING
//------------------------------------------------------------------------
		
} // end serialization namespace

//------------------------------------------------------------------------
// iostream operators - Definitions
//------------------------------------------------------------------------
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

std::istream& operator >> ( std::istream& sin, c_script& scr ) {
	sin
		>> scr.name;
	return sin;
}

//------------------------------------------------------------------------
// Evaluation Functions
//------------------------------------------------------------------------
std::ostream& operator << ( std::ostream& sout, const c_scriptEvaluation& scr ) {
	sout
		<< scr.getScriptType() << " "
		<< scr.getScriptSubType() << " "
		<< scr.name << " "
		<< scr.evalType << " "
		<< scr.getReturnVal();
		//<< (void*)scr.getVarToEvaluate() << " "
		//<< (void*)scr.getVarToReference() << " ";
		
	return sout;
}

std::istream& operator >> ( std::istream& sin, c_scriptEvaluation& scr ) {
	c_scriptVarBase* evalVar( NULL );
	c_scriptVarBase* refVar( NULL );
	sin
		>> scr.name
		>> scr.evalType
		>> scr.returnVal;
		//varToEvaluate and varToReference are neither read in or saved
		//this is so that there can be a "master" serialization function
		//to manage the saving and loading of pointers
		
		scr.attachVarToEvaluate( evalVar );
		scr.attachVarToReference( refVar );
		
	return sin;
}

//------------------------------------------------------------------------
// String Variables
//------------------------------------------------------------------------
std::ostream& operator << ( std::ostream& sout, const c_scriptString& scr ) {
	sout
		<< scr.getScriptType() << " "
		<< scr.getScriptSubType() << " "
		<< scr.name
		<< scr.variable << '\0';
		//very important NULL-termination. Necessary for reading data back from files
	return sout;
}

std::istream& operator >> ( std::istream& sin, c_scriptString& scr ) {
	sin
		>> scr.name;
	
	std::streampos string_begin;
	std::string::size_type num_chars( 0 );
	string_begin = sin.tellg();
	char null_term( 1 );
	
	while ( null_term != 0 ) {
		null_term = sin.get();
	}
	
	//I have tested this extensively using G++ & MSVC++.
	//It NULL-terminates and gives the correct string size.
	num_chars = (sin.tellg() - string_begin) - 1;
	scr.variable.resize( num_chars );
	sin.read( (char*)scr.variable.data(), num_chars );
	scr.variable[ num_chars ] = '\0';
	
	return sin;
}

} // end harbinger namespace