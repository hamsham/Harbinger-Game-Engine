
//Scripting prototype (again)
#ifndef __SCRIPT_TEST_HEADER__
#define __SCRIPT_TEST_HEADER__

//#include's are located at the bottom of the header.

namespace harbinger {

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
	
	SCRIPT_VAR_MAX
};
/* None of these are anywhere close to being implemented
enum e_scriptResourceType {
	SCRIPT_RES_BITMAP,
	SCRIPT_RES_SPRITE,
	SCRIPT_RES_SOUND,
	SCRIPT_RES_POLYGON,
	SCRIPT_RES_MESH,
	SCRIPT_RES_SKELETON,
	SCRIPT_RES_ANIMATION,
};
*/
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
template <typename type> class c_scriptVar; //inherits c_scriptVarBase
class c_scriptNum; //empty base class, inherits nothing
class c_scriptInt;
class c_scriptUint;
class c_scriptFloat;
class c_scriptBool;
class c_scriptString;
class c_scriptVec3;

//functions
template <typename returnType> class c_scriptFunc; //abstract base,  inherits c_scriptFuncBase
class c_scriptEvaluation; //abstract, inherits c_scriptFunction, has a boolean result
class c_scriptNumeric;
class c_scriptNumEval; //inherits c_scriptEval, has a boolean result
class c_scriptMiscMath; //inherits c_scriptFunc. has a float result
class c_scriptArithmetic; //inherits c_scriptFunc. has a c_scriptNum result. takes two arguments (except for the EQL function, which has one)
class c_scriptTrigonometry; //inherits c_scriptFunc. has a float result in radians. takes one argument

} //end harbinger namespace

#endif //__SCRIPT_TEST_HEADER__