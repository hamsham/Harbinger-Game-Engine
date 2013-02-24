/* 
 * File:   script_manager.cpp
 * Author: hammy
 *
 * Created on February 16, 2013, 9:48 PM
 */
#include <iostream>
#include "scripting/script.h"
#include "scripting/script_base.h"
#include "scripting/script_serializer.h"
#include "scripting/script_factory.h"
#include "scripting/script_manager.h"

///////////////////////////////////////////////////////////////////////////////
//		Scripting Manager
///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
// Script Manager -- Constructor
//-----------------------------------------------------------------------------
c_scriptManager::c_scriptManager() :
	scriptFile(),
	scriptMap(),
	scriptList()
{}

// design choice -- leaving the copy constructor up to STL
c_scriptManager::c_scriptManager( const c_scriptManager& sm ) :
	scriptFile( sm.scriptFile ),
	scriptMap( sm.scriptMap ),
	scriptList( sm.scriptList )
{}

//-----------------------------------------------------------------------------
// Script Manager -- File Data
//-----------------------------------------------------------------------------
const std::string& c_scriptManager::getFileName() const {
	return scriptFile;
}

void c_scriptManager::setFileName( const std::string& inName ) {
	scriptFile = inName;
}

bool c_scriptManager::hasFile() const {
	return scriptFile.size() > 0;
}

//-----------------------------------------------------------------------------
// Script Manager -- Serialization
//-----------------------------------------------------------------------------
bool c_scriptManager::save() const {
	c_serializer::e_fileStatus fileStatus;
	c_serializer serializer;
	
	fileStatus = serializer.saveScripts( scriptFile.c_str(), scriptList, true );
	
	if ( fileStatus == c_serializer::FILE_SAVE_SUCCESS ) 
		return true;
	
	std::cerr << "ERROR: Unable to save scriptFile due to error " << fileStatus << std::endl;
	return false;
}

bool c_scriptManager::load() {
	c_serializer::e_fileStatus fileStatus;
	c_serializer serializer;
	
	fileStatus = serializer.loadScripts( scriptFile.c_str(), scriptList );
	
	if ( fileStatus == c_serializer::FILE_LOAD_SUCCESS ) 
		return true;
	
	std::cerr << "ERROR: Unable to load scriptFile due to error " << fileStatus << std::endl;
	return false;
}

//-----------------------------------------------------------------------------
// Script Manager -- Data Access
//-----------------------------------------------------------------------------
const c_script* c_scriptManager::operator []( uint index ) const {
	HGE_ASSERT( (index >= 0) && (index < scriptList.size()) );
	return scriptList[ index ];
}

c_script* c_scriptManager::operator []( uint index ) {
	HGE_ASSERT( (index >= 0) && (index < scriptList.size()) );
	return scriptList[ index ];
}

//-----------------------------------------------------------------------------
// Script Manager -- Entry Management
//-----------------------------------------------------------------------------
void c_scriptManager::addEntry( c_script* script ) {
	// check to make sure that "script" isn't already being managed
	scriptMap_t::iterator iter = scriptMap.find( (void*)script );
	if ( iter != scriptMap.end() ) return;
	
	c_script* pScript( HGE_NULL );
	if ( script->getScriptType() == SCRIPT_VAR ) {
		pScript = n_scriptFactory::getVarInstance( script->getScriptSubType() );
	}
	else if ( script->getScriptType() == SCRIPT_FUNC ) {
		pScript = n_scriptFactory::getFuncInstance( script->getScriptSubType() );
	}
	
	if ( pScript == HGE_NULL ) return;
	
	*pScript = *script;
	scriptList.push_back( pScript );
	scriptMap[ (void*)pScript ] = pScript;
}

void c_scriptManager::manageEntry( c_script* script ) {
	// check to make sure that "script" isn't already being managed
	scriptMap_t::iterator iter = scriptMap.find( (void*)script );
	if ( iter != scriptMap.end() ) return;
	
	scriptList.push_back( script );
	scriptMap[ (void*)script ] = script;
}

void c_scriptManager::popEntry( unsigned int index ) {
	HGE_ASSERT( (index >= 0) && (index < scriptList.size()) );
	
	c_script* pScript( HGE_NULL );
	scriptList_t::iterator listIter = scriptList.begin() + index;
	pScript = *listIter;
	scriptMap.erase( (void*)pScript );
	
	delete pScript;
	*listIter = HGE_NULL;
}

//-----------------------------------------------------------------------------
// Script Manager -- Clearance
//-----------------------------------------------------------------------------
void c_scriptManager::clearEntries() {
	scriptMap.clear();
	for ( scriptListSize_t i( 0 ); i < scriptList.size(); ++i ) {
		n_scriptFactory::killInstance( scriptList[ i ] );
		scriptList[ i ] = HGE_NULL;
	}
	scriptList.clear();
}
