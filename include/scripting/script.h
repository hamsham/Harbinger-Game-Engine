
#ifndef __HGE_SCRIPT_H__
#define __HGE_SCRIPT_H__

#include <string>
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

template <e_varType, typename>
class c_var;

template <e_funcType, int numArgs, typename returntype>
class c_func;

///////////////////////////////////////////////////////////////////////////////
//		Typedefs
///////////////////////////////////////////////////////////////////////////////
// Object Variables
typedef c_var< SCRIPT_VAR_INT, int >			c_varInt;
typedef c_var< SCRIPT_VAR_FLOAT, float >		c_varFloat;
typedef c_var< SCRIPT_VAR_BOOL, bool >			c_varBool;
typedef c_var< SCRIPT_VAR_BOOL, vec3 >			c_varVec3;
typedef c_var< SCRIPT_VAR_BOOL, std::string >	c_varString;

// Integer Functions
typedef c_func< SCRIPT_FUNC_INT_ADD, 2, c_varInt >			c_fncIntAdd;
typedef c_func< SCRIPT_FUNC_INT_SUB, 2, c_varInt >			c_fncIntSub;
typedef c_func< SCRIPT_FUNC_INT_MUL, 2, c_varInt >			c_fncIntMul;
typedef c_func< SCRIPT_FUNC_INT_DIV, 2, c_varInt >			c_fncIntDiv;
typedef c_func< SCRIPT_FUNC_INT_MOD, 2, c_varInt >			c_fncIntMod;
typedef c_func< SCRIPT_FUNC_INT_EQL, 2, c_varInt >			c_fncIntEql;

// Float Functions
typedef c_func< SCRIPT_FUNC_FLOAT_ADD, 2, c_varFloat >		c_fncFloatAdd;
typedef c_func< SCRIPT_FUNC_FLOAT_SUB, 2, c_varFloat >		c_fncFloatSub;
typedef c_func< SCRIPT_FUNC_FLOAT_MUL, 2, c_varFloat >		c_fncFloatMul;
typedef c_func< SCRIPT_FUNC_FLOAT_DIV, 2, c_varFloat >		c_fncFloatDiv;
typedef c_func< SCRIPT_FUNC_FLOAT_MOD, 2, c_varFloat >		c_fncFloatMod;
typedef c_func< SCRIPT_FUNC_FLOAT_EQL, 2, c_varFloat >		c_fncFloatEql;

typedef c_func< SCRIPT_FUNC_INT_CAST, 1, c_varInt >			c_fncIntCast;
typedef c_func< SCRIPT_FUNC_FLOAT_CAST, 1, c_varFloat >		c_fncFloatCast;

typedef c_func< SCRIPT_FUNC_NUM_ROUND, 1, c_varFloat >		c_fncNumRound;

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