/* 
 * File:   resource.cpp
 * Author: hammy
 * 
 * Created on September 9, 2012, 9:35 PM
 */

#include "../../include/harbinger.h"
namespace harbinger {

//-----------------------------------------------------------------------------
//		Scripting Manager
//-----------------------------------------------------------------------------
const char* c_scriptManager::scriptVarCfgSection = "VARIABLES";
const char* c_scriptManager::scriptFuncCfgSection = "FUNCTIONS";
/*
//construction & destruction
c_scriptManager::c_scriptManager() {}

c_scriptManager::c_scriptManager( const c_scriptManager& smCopy ) {
	throw "Script Manager copying not yet implemented!";
}

c_scriptManager::~c_scriptManager() {
	clear();
}
		
c_scriptManager& c_scriptManager::operator = ( const c_scriptManager& smCopy ) {
	throw "Script Manager copying not yet implemented!";
}

//object addition & removal
const char* c_scriptManager::addResource( const c_script& objToAdd ) {
	if (resourceArray.size() == resourceArray.max_size()) return nullptr;
	s_resource newResource;
	
	//massive switch statement to determine what type of class to add
	//these statements convert objToAdd from a c_script type back into
	//the script type that it actually is. reinterpret cast is safe in this circumstance
	
	//script variables
	if ( objToAdd.getScriptType() == SCRIPT_VAR ) {
		switch ( objToAdd.getScriptSubType() ) {
			case SCRIPT_VAR_INT:
				newResource.resource = new c_scriptInt( *(reinterpret_cast< c_scriptInt* >( &objToAdd)) );
				break;
			case SCRIPT_VAR_UINT:
				newResource.resource = new c_scriptUint( *(reinterpret_cast< c_scriptUint* >( &objToAdd)) );
				break;
			case SCRIPT_VAR_FLOAT:
				newResource.resource = new c_scriptFloat( *(reinterpret_cast< c_scriptFloat* >( &objToAdd)) );
				break;
			case SCRIPT_VAR_STRING:
				newResource.resource = new c_scriptString( *(reinterpret_cast< c_scriptString* >( &objToAdd)) );
				break;
			case SCRIPT_VAR_BOOL:
				newResource.resource = new c_scriptBool( *(reinterpret_cast< c_scriptBool* >( &objToAdd)) );
				break;
			default:
				return nullptr;
		}
	}
	//script functions
	else if ( objToAdd.getScriptType() == SCRIPT_FUNC ) {
		switch ( objToAdd.getScriptSubType() ) {
			case SCRIPT_FUNC_NUM_EVAL:
				newResource.resource = new c_scriptNumEval( *(reinterpret_cast< c_scriptNumEval* >( &objToAdd)) );
				break;
			case SCRIPT_FUNC_NUM_ARITH:
				newResource.resource = new c_scriptMiscMath( *(reinterpret_cast< c_scriptMiscMath* >( &objToAdd)) );
				break;
			case SCRIPT_FUNC_NUM_TRIG:
				newResource.resource = new c_scriptArithmetic( *(reinterpret_cast< c_scriptArithmetic* >( &objToAdd)) );
				break;
			case SCRIPT_FUNC_NUM_MISC:
				newResource.resource = new c_scriptTrigonometry( *(reinterpret_cast< c_scriptTrigonometry* >( &objToAdd)) );
				break;
			default:
				return nullptr;
		}
	}
	
	//add the resource to the dictionary
	createResourceID( newResource );
	referenceDict.addWord( newResource.resourceID.c_str() );
	referenceDict.setDefinition(
		newResource.resourceID.c_str(), newResource
	);
	//get a reference to the object in the dictionary and add it to the array
	s_resource* pResource;
	pResource = referenceDict.getDefinition( newResource.resourceID );
	resourceArray.push_back( pResource );
	pResource->resourceIndex = resourceArray.size() - 1;
	
	return pResource->resourceID.c_str();
}

//serialization
hamLibs::config::configSaveStatus c_scriptManager::save() const {
	std::ostringstream stout;
	hamLibs::config cfg;
	c_script* pScript( nullptr );
	
	cfg.addSection( scriptVarCfgSection );
	cfg.addSection( scriptFuncCfgSection );
	
	for (size_t i( 0 ); i < resourceArray[ i ]; ++i) {
		pScript = resourceArray[ i ]->resource;
		
		stout << pScript->getScriptType() << " " << pScript->name;
		stout << " " << pScript->getScriptSubType();
		
		if ( pScript->getScriptType() == SCRIPT_VAR ) {
			stout << reinterpret_cast< c_scriptVar* >(pScript)->data();
		}
		
		//for functions, don't save the return value. store the array indices of member variables
		//this will make deseralization much easier
		else if ( pScript->getScriptType() == SCRIPT_FUNC ) {
			
			if ( pScript->getScriptSubType() == SCRIPT_FUNC_NUM_EVAL ) {
				c_scriptNumEval* pEvalFunc = reinterpret_cast< c_scriptNumEval* >(pScript);
				stout << " " << pEvalFunc->evalType;
				stout << " " << getResourceIndex( pEvalFunc->varToEval );
				stout << " " << getResourceIndex( pEvalFunc->varToRef );
			}
			else if ( pScript->getScriptSubType() == SCRIPT_FUNC_NUM_MISC ) {
				c_scriptMiscMath* pMathFunc = reinterpret_cast< c_scriptMiscMath* >(pScript);
				stout << " " << pMathFunc->evalType;
				stout << " " << getResourceIndex( pMathFunc->evalNum );
			}
			else if ( pScript->getScriptSubType() == SCRIPT_FUNC_NUM_ARITH ) {
				c_scriptArithmetic* pMathFunc = reinterpret_cast< c_scriptArithmetic* >(pScript);
				stout << " " << pMathFunc->evalType;
				stout << " " << getResourceIndex( pMathFunc->evalNum );
				stout << " " << getResourceIndex( pMathFunc->refNum );
			}
			else if ( pScript->getScriptSubType() == SCRIPT_FUNC_NUM_TRIG ) {
				c_scriptTrigonometry* pMathFunc = reinterpret_cast< c_scriptTrigonometry* >(pScript);
				stout << " " << pMathFunc->evalType;
				stout << " " << getResourceIndex( pMathFunc->evalNum );
			}
			else {
				//nothing was added. indicates an invalid value
				break;
			}
		}
	}
}
*/
} // end harbinger namespace
