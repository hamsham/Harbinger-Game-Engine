/* 
 * File:   serialize.h
 * Author: hammy
 *
 * Created on October 27, 2012, 11:24 PM
 */

#ifndef __HGE_SERIALIZE_H__
#define	__HGE_SERIALIZE_H__

namespace harbinger {

//------------------------------------------------------------------------
//	SERIALIZATION CLASS
//------------------------------------------------------------------------
namespace serialization {
	//file statuses
	enum e_fileStatus {
		FILE_LOAD_ERROR,
		FILE_LOAD_INVALID_NAME,
		FILE_LOAD_INVALID_PATH,
		FILE_LOAD_IO_ERROR,
		FILE_LOAD_INVALID_DATA,
		FILE_LOAD_SUCCESS,

		FILE_SAVE_ERROR,
		FILE_SAVE_INVALID_NAME,
		FILE_SAVE_INVALID_PATH,
		FILE_SAVE_IO_ERROR,
		FILE_SAVE_OVERWRITE,
		FILE_SAVE_INVALID_DATA,
		FILE_SAVE_SUCCESS
	};
	
	//officially supported file types
	enum e_hgeFileType {
		SCRIPT_FILE = 0,	// contains a list of scripts to load
		RESOURCE_FILE,		// contains a list of resources to load (compounds the script files)
		PERSISTENT_FILE,	//  contains data persistent to all areas of the game being loaded
		FILE_TYPE_MAX
	};
	const char* const HARBINGER_FILE_TYPES[ FILE_TYPE_MAX ] = {
		"hsd", // harbinger script data
		"hrl", // harbinger resource list
		"hpr", // harbinger persistent resource
	};
	
	//script saving routines
	e_fileStatus saveScripts(
		const char* fileName,
		e_hgeFileType fileType,
		scriptList& inScripts,
		bool overwriteData = true
	);
	
	//script loading routines
	e_fileStatus loadScripts(
		const char* fileName,
		e_hgeFileType fileType,
		scriptList& outScripts
	);

} // end serialization namespace

//------------------------------------------------------------------------
// iostream operators - forward declarations
//------------------------------------------------------------------------
/* A note about the stream operators:
* The script type and sub-type will be printed when being sent to an ostream
* but they are not read back in by an istream.
* This is because the script type and sub-type must be determined before
* reading in any object data. This makes it much easier to determine what
* type of polymorphic object should be loaded when saving to/loading from files
*/
// c_script base class
std::ostream& operator << ( std::ostream&, const c_script& );
std::istream& operator >> ( std::istream&, c_script& );

// script variables
template <typename type> std::ostream& operator << ( std::ostream&, const c_scriptVar<type>& );
template <typename type> std::istream& operator >> ( std::istream&, c_scriptVar<type>& );

// String Variables
std::ostream& operator << ( std::ostream&, const c_scriptString& );
std::istream& operator >> ( std::istream&, c_scriptString& );

// evaluation-type functions
std::ostream& operator << ( std::ostream&, const c_scriptEvaluation& );
std::istream& operator >> ( std::istream&, c_scriptEvaluation& );

// numerical evaluations
std::ostream& operator << ( std::ostream&, const c_scriptNumeric& );
std::istream& operator >> ( std::istream&, c_scriptNumeric& );

//------------------------------------------------------------------------
// iostream operators - templated definitions
//------------------------------------------------------------------------
//script variables
template <typename type>
std::ostream& operator << ( std::ostream& sout, const c_scriptVar<type>& scr ) {
	sout
		<< scr.getScriptType() << " "
		<< scr.getScriptSubType() << " "
		<< scr.getName() << " "
		<< scr.variable;
	return sout;
}

template <typename type>
std::istream& operator >> ( std::istream& sin, c_scriptVar<type>& scr ) {
	std::string varName;
	sin
		>> varName
		>> scr.variable;
		scr.setName( varName );
	return sin;
}

} // end harbinger namespace
#endif	/* __HGE_SERIALIZE_H__ */
