
#ifndef __HGE_SCRIPT_H__
#define __HGE_SCRIPT_H__

#include <vector>
#include <map>

#include "../types.h"

#include "utils/hash.h"
using hamLibs::utils::constHash;

#define HASHED_ENUM( x ) x = (unsigned short)constHash( HL_STRINGIFY( x ) )

///////////////////////////////////////////////////////////////////////////////
//		Object Types
///////////////////////////////////////////////////////////////////////////////
enum e_objType : long int {
	HASHED_ENUM( SCRIPT_VAR ),
	HASHED_ENUM( SCRIPT_FUNC )
};

enum e_varType : long int {
	HASHED_ENUM( SCRIPT_VAR_INT ),
	HASHED_ENUM( SCRIPT_VAR_FLOAT ),
	HASHED_ENUM( SCRIPT_VAR_BOOL ),
	HASHED_ENUM( SCRIPT_VAR_VEC3 ),
	HASHED_ENUM( SCRIPT_VAR_STRING )
};

enum e_funcType : long int {
	HASHED_ENUM( SCRIPT_FUNC_INT_ADD ),
	HASHED_ENUM( SCRIPT_FUNC_INT_SUB ),
	HASHED_ENUM( SCRIPT_FUNC_INT_MUL ),
	HASHED_ENUM( SCRIPT_FUNC_INT_DIV ),
	HASHED_ENUM( SCRIPT_FUNC_INT_MOD ),
	HASHED_ENUM( SCRIPT_FUNC_INT_EQL ),
	
	HASHED_ENUM( SCRIPT_FUNC_FLOAT_ADD ),
	HASHED_ENUM( SCRIPT_FUNC_FLOAT_SUB ),
	HASHED_ENUM( SCRIPT_FUNC_FLOAT_MUL ),
	HASHED_ENUM( SCRIPT_FUNC_FLOAT_DIV ),
	HASHED_ENUM( SCRIPT_FUNC_FLOAT_MOD ),
	HASHED_ENUM( SCRIPT_FUNC_FLOAT_EQL ),
	
	HASHED_ENUM( SCRIPT_FUNC_INT_CAST ),
	HASHED_ENUM( SCRIPT_FUNC_FLOAT_CAST ),
	
	HASHED_ENUM( SCRIPT_FUNC_NUM_ROUND )
};

///////////////////////////////////////////////////////////////////////////////
//		Forward Declarations
///////////////////////////////////////////////////////////////////////////////
class c_script;
class c_varBase;
class c_funcBase;

template < e_varType varType, typename dataType >
class c_var;

template < e_funcType, int numArgs, typename returntype >
class c_func;

//-----------------------------------------------------------------------------
//		Management & Serialization
//-----------------------------------------------------------------------------
class c_scriptManager;
class c_serializer;

//-----------------------------------------------------------------------------
//		Serialization Support
//-----------------------------------------------------------------------------
//Scripting Containers & Structures
typedef std::map< void*, c_script* >	scriptMap_t;
typedef std::map< void*, c_varBase* >	scriptVarMap_t;
typedef std::map< void*, c_funcBase* >	scriptFuncMap_t;
typedef std::vector< c_script* >		scriptList_t;
typedef scriptList_t::size_type			scriptListSize_t;

#undef HASHED_ENUM
#endif //__HGE_SCRIPT_H__