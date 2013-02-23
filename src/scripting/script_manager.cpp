/* 
 * File:   script_manager.cpp
 * Author: hammy
 *
 * Created on February 16, 2013, 9:48 PM
 */
#include <iostream>
#include "scripting/script_base.h"
#include "scripting/script_functions.h"
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
// Script Manager Namespace -- Instances
//-----------------------------------------------------------------------------
c_script* c_scriptManager::getVarInstance( long scriptType ) {
	switch( scriptType ) {
		case SCRIPT_VAR_INT:
			return new c_varInt;
		case SCRIPT_VAR_FLOAT:
			return new c_varFloat;
		case SCRIPT_VAR_BOOL:
			return new c_varBool;
		case SCRIPT_VAR_STRING:
			return new c_varString;
		case SCRIPT_VAR_VEC3:
			return new c_varVec3;
		default:
			return HGE_NULL;
	}
}

c_script* c_scriptManager::getFuncInstance( long scriptType ) {
	switch( scriptType ) {
		
		case SCRIPT_FUNC_INT_ADD:
			return new c_fncIntAdd;
		case SCRIPT_FUNC_INT_SUB:
			return new c_fncIntSub;
		case SCRIPT_FUNC_INT_MUL:
			return new c_fncIntMul;
		case SCRIPT_FUNC_INT_DIV:
			return new c_fncIntDiv;
		case SCRIPT_FUNC_INT_MOD:
			return new c_fncIntMod;
		case SCRIPT_FUNC_INT_EQL:
			return new c_fncIntEql;
		
		case SCRIPT_FUNC_FLOAT_ADD:
			return new c_fncFloatAdd;
		case SCRIPT_FUNC_FLOAT_SUB:
			return new c_fncFloatSub;
		case SCRIPT_FUNC_FLOAT_MUL:
			return new c_fncFloatMul;
		case SCRIPT_FUNC_FLOAT_DIV:
			return new c_fncFloatDiv;
		case SCRIPT_FUNC_FLOAT_MOD:
			return new c_fncFloatMod;
		case SCRIPT_FUNC_FLOAT_EQL:
			return new c_fncFloatEql;
			
		case SCRIPT_FUNC_INT_CAST:
			return new c_fncIntCast;
		case SCRIPT_FUNC_FLOAT_CAST:
			return new c_fncFloatCast;
			
		case SCRIPT_FUNC_NUM_ROUND:
			return new c_fncNumRound;
			
		default:
			return HGE_NULL;
	}
}

void c_scriptManager::killInstance( c_script* s ) {
	delete s;
	s = HGE_NULL;
}

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
		pScript = getVarInstance( script->getScriptSubType() );
	}
	else if ( script->getScriptType() == SCRIPT_FUNC ) {
		pScript = getFuncInstance( script->getScriptSubType() );
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
		delete scriptList[ i ];
		scriptList[ i ] = HGE_NULL;
	}
	scriptList.clear();
}
