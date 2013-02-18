
#ifndef __HGE_SCRIPT_H__
#define __HGE_SCRIPT_H__

#include <vector>
#include <map>
#include "../types.h"

//-----------------------------------------------------------------------------
//		Script Types
// The following order of enumerations is VERY important for the rest of the
// engine. Whenever a new class is added, ALWAYS add a new enum type to the
// bottom of its corresponding list.
//-----------------------------------------------------------------------------
enum e_scriptType : signed int { 
	//base classes
	SCRIPT_INVALID = -1,
	SCRIPT_BASE = SCRIPT_INVALID,
	SCRIPT_VAR = 0,
	SCRIPT_FUNC = 1,
	SCRIPT_MAX
};

enum e_scriptVarType : signed int {
	SCRIPT_VAR_INVALID = SCRIPT_INVALID,
	SCRIPT_VAR_NUM = SCRIPT_INVALID,
	SCRIPT_VAR_INT = 0,
	SCRIPT_VAR_UINT = 1,
	SCRIPT_VAR_FLOAT = 2,
	SCRIPT_VAR_BOOL = 3,
	SCRIPT_VAR_STRING = 4,
	SCRIPT_VAR_VEC3 = 5,
	SCRIPT_VAR_RESOURCE = 6, // defined in "resource.h"
	
	SCRIPT_VAR_MAX
};

enum e_scriptFuncType : signed int {
	SCRIPT_FUNC_INVALID = SCRIPT_INVALID,
	SCRIPT_FUNC_EVAL = SCRIPT_INVALID,
	SCRIPT_FUNC_NUMERICAL = SCRIPT_INVALID,
	SCRIPT_FUNC_NUM_EVAL = 0,
	SCRIPT_FUNC_NUM_MISC = 1,
	SCRIPT_FUNC_NUM_ARITH = 2,
	SCRIPT_FUNC_NUM_TRIG = 3,
	//SCRIPT_FUNC_STRING_EVAL, //NOT YET IMPLEMENTED
	
	SCRIPT_FUNC_MAX
};

//-----------------------------------------------------------------------------
//		Forward Declarations
//-----------------------------------------------------------------------------
//base classes, all abstract
class c_script;
class c_scriptVarBase;
class c_scriptFuncBase;

//variables
template <typename type> class c_scriptVar; // abstract, inherits c_scriptVarBase
template <typename type> class c_scriptResource; //abstract, inherits c_scriptVar
class c_scriptNum; //empty base class, inherits only c_script
class c_scriptInt; // c_scriptNum
class c_scriptUint; // Num
class c_scriptFloat; // Num
class c_scriptBool; // c_scriptVar
class c_scriptString; // Var
class c_scriptVec3; // Var

//functions
template <typename returnType> class c_scriptFunc; //abstract,  inherits c_scriptFuncBase
class c_scriptEvaluation; //abstract, inherits c_scriptFunc, has a boolean result
class c_scriptNumeric; //abstract, inherits c_scriptFunc, has a float result
class c_scriptNumEval; //inherits c_scriptEval
class c_scriptMiscMath; //inherits c_scriptFunc
class c_scriptArithmetic; //inherits c_scriptFunc
class c_scriptTrigonometry; //inherits c_scriptFunc

//-----------------------------------------------------------------------------
//		Management & Serialization
//-----------------------------------------------------------------------------
class c_scriptManager;
class c_serializer;

//-----------------------------------------------------------------------------
//		Serialization Support
//-----------------------------------------------------------------------------
//Scripting Containers & Structures
typedef std::map< void*, c_script* >			scriptMap_t;
typedef std::map< void*, c_scriptVarBase* >		scriptVarMap_t;
typedef std::map< void*, c_scriptFuncBase* >	scriptFuncMap_t;
typedef std::vector< c_script* >				scriptList_t;
typedef scriptList_t::size_type					scriptListSize_t;

#endif //__HGE_SCRIPT_H__