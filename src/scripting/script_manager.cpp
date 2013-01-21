/* 
 * File:   resource.cpp
 * Author: hammy
 * 
 * Created on November 4, 2012, 9:21 PM
 */

#include <string>
#include "scripting/script_manager.h"

//-----------------------------------------------------------------------------
//		Script Resource Variable Base Class
//-----------------------------------------------------------------------------
c_resource::c_resource( const std::string& fileName ) :
	resFile( fileName )
{}

c_resource::c_resource( const c_resource& resCopy ) :
	resFile( resCopy.resFile )
{}

bool c_resource::setFile( const std::string& fileName ) {
	if ( al_filename_exists( fileName.c_str() ) == false )
		return false;
	
	if ( resFile != fileName && isLoaded() == true ) {
		resFile = fileName;
		return reload();
	}
	//else
	resFile = fileName;
	return true;
}

const std::string& c_resource::getFile() const {
	return resFile;
}

//-----------------------------------------------------------------------------
//		Script Management
//-----------------------------------------------------------------------------
c_scriptManager::~c_scriptManager() {
	unloadScripts();
}
