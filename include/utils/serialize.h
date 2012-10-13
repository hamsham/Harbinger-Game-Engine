/* 
 * File:   serialize.h
 * Author: hammy
 *
 * Created on September 9, 2012, 2:20 PM
 */

#ifndef	__HGE_SERIALIZE_H__
#define	__HGE_SERIALIZE_H__

namespace harbinger {
	
//Endianness Check

namespace serialize {
	enum e_saveState {
		SERIALIZE_SAVE_IO_ERROR,
		SERIALIZE_SAVE_OVERWRITE,
		SERIALIZE_SAVE_INVALID_FILE,
		SERIALIZE_SAVE_SUCCESS
	};

	enum e_loadState {
		SERIALIZE_LOAD_NOT_OPEN,
		SERIALIZE_LOAD_IO_ERROR,
		SERIALIZE_LOAD_INVALID_FILE,
		SERIALIZE_LOAD_SUCCESS
	};

	//Script Saving Routines
	hamLibs::config::configSaveStatus	saveToConfig	(
		const char* cfgFile,
		const std::vector< c_script >& scriptArray
	);
	hamLibs::config::configSaveStatus	saveToConfig	(
		const char* cfgFile,
		const c_script& script
	);

	//Script Loading Routines
	hamLibs::config::configLoadStatus	loadFromConfig	(
		const char* cfgFile,
		std::vector< c_script >& scriptArray
	);

}; // end c_serializer

} // end harbinger namespace

#endif	/* __HGE_SERIALIZE_H__ */
