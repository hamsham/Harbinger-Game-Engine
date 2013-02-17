/* 
 * File:   serialize.h
 * Author: hammy
 *
 * Created on October 27, 2012, 11:24 PM
 */

#ifndef __HGE_SCRIPT_SERIALIZE_H__
#define	__HGE_SCRIPT_SERIALIZE_H__

#include "script.h"

//officially supported file types
enum e_hgeFileType : int {
	HGE_SCRIPT_FILE_INVALID = -1,
	HGE_SCRIPT_RAW_DATA = 0,		// contains a list of scripts to load
	HGE_SCRIPT_EDITOR_DATA = 1	// contains the above, plus GUI data & names
};

cstr const HARBINGER_FILE_TYPE[] = {
	"hsd", // harbinger script data
	"hed" // harbinger editor data
};

//------------------------------------------------------------------------
//	SERIALIZATION CLASS
//------------------------------------------------------------------------
class HGE_API c_serializer {
	private:
		
		e_hgeFileType getFileType( cstr fileName ) const;
		bool readHeader( std::ifstream&, scriptList_t& );
		bool readFooter( std::ifstream&, scriptList_t& inScripts, hgeSize_t numVars, hgeSize_t numFuncs );
		
		void unloadData( scriptList_t& ); //in case of a file read error, clear memory
		//void closeStream();
		
	public:
		//file statuses
		enum e_fileStatus {
			FILE_LOAD_ERROR,
			FILE_LOAD_INVALID_TYPE,
			FILE_LOAD_INVALID_NAME,
			FILE_LOAD_INVALID_PATH,
			FILE_LOAD_IO_ERROR,
			FILE_LOAD_OVERWRITE,
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
		
		e_fileStatus saveScripts( cstr fileName, const scriptList_t& inScripts, bool overwriteData = true );
		e_fileStatus loadScripts( cstr fileName, scriptList_t& outScripts );
}; // end serialization class

#endif	/* __HGE_SCRIPT_SERIALIZE_H__ */
