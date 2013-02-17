/* 
 * File:   script_manager.cpp
 * Author: hammy
 *
 * Created on February 16, 2013, 9:48 PM
 */

#include "scripting/script.h"
#include "scripting/script_base.h"
#include "scripting/script_variables.h"
#include "scripting/script_functions.h"
#include "scripting/script_serializer.h"
#include "scripting/script_manager.h"

//-----------------------------------------------------------------------------
// Script Factories
//-----------------------------------------------------------------------------
c_script* getVarInstance( int scriptType ) {
	switch( scriptType ) {
		case SCRIPT_VAR_INT:
			return new( std::nothrow ) c_scriptInt;
		case SCRIPT_VAR_UINT:
			return new( std::nothrow ) c_scriptUint;
		case SCRIPT_VAR_FLOAT:
			return new( std::nothrow ) c_scriptFloat;
		case SCRIPT_VAR_BOOL:
			return new( std::nothrow ) c_scriptBool;
		case SCRIPT_VAR_STRING:
			return new( std::nothrow ) c_scriptString;
		case SCRIPT_VAR_VEC3:
			return new( std::nothrow ) c_scriptVec3;
		default:
			return nullptr;
	}
}

c_script* getFuncInstance( int scriptType ) {
	switch( scriptType ) {
		case SCRIPT_FUNC_NUM_EVAL:
			return new( std::nothrow ) c_scriptNumEval;
		case SCRIPT_FUNC_NUM_MISC:
			return new( std::nothrow ) c_scriptMiscMath;
		case SCRIPT_FUNC_NUM_ARITH:
			return new( std::nothrow ) c_scriptArithmetic;
		case SCRIPT_FUNC_NUM_TRIG:
			return new( std::nothrow ) c_scriptTrigonometry;
		default:
			return nullptr;
	}
}

void c_scriptManager::killInstance( c_script* s ) {
	delete s;
	s = nullptr;
}

