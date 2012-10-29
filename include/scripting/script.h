
//Scripting prototype (again)
#ifndef __SCRIPT_TEST_HEADER__
#define __SCRIPT_TEST_HEADER__

namespace harbinger {

//-----------------------------------------------------------------------------
//		Script Types
//-----------------------------------------------------------------------------
enum e_scriptType { 
	//base classes
	SCRIPT_BASE = 0,
	SCRIPT_VAR,
	SCRIPT_FUNC,
	SCRIPT_INVALID
};

enum e_scriptVarType {
	SCRIPT_VAR_INVALID = 0,
	SCRIPT_VAR_NUM,
	SCRIPT_VAR_INT,
	SCRIPT_VAR_UINT,
	SCRIPT_VAR_FLOAT,
	SCRIPT_VAR_BOOL,
	SCRIPT_VAR_STRING,
	SCRIPT_VAR_VEC3 //NOT YET IMPLEMENTED
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
enum e_scriptFuncType {
	SCRIPT_FUNC_INVALID = 0,
	SCRIPT_FUNC_EVAL,
	SCRIPT_FUNC_NUM_EVAL,
	SCRIPT_FUNC_NUM_ARITH,
	SCRIPT_FUNC_NUM_TRIG,
	SCRIPT_FUNC_NUM_MISC,
	SCRIPT_FUNC_STRING_EVAL //NOT YET IMPLEMENTED
};

//-----------------------------------------------------------------------------
//		Forward Declarations
//-----------------------------------------------------------------------------
//base classes, all abstract
class c_script;
class c_scriptVarBase;
class c_scriptFuncBase;

//variables
template <typename type>
class c_scriptVar; //inherits c_scriptVarBase
class c_scriptNum; //empty base class, inherits nothing
class c_scriptInt;
class c_scriptUint;
class c_scriptFloat;
class c_scriptString; //NOT IMPLEMENTED YET
class c_scriptBool; //NOT IMPLEMENTED YET
class c_scriptBitmap; //NOT IMPLEMENTED YET

//functions
template <typename returnType>
class c_scriptFunc; //abstract base,  inherits c_scriptFuncBase
class c_scriptEvaluation; //abstract, inherits c_scriptFunction, has a boolean result
class c_scriptNumEval; //inherits c_scriptEval, has a boolean result
class c_scriptMiscMath; //inherits c_scriptFunc. has a float result
class c_scriptArithmetic; //inherits c_scriptFunc. has a c_scriptNum result. takes two arguments (except for the EQL function, which has one)
class c_scriptTrigonometry; //inherits c_scriptFunc. has a float result in radians. takes one argument

} //end harbinger namespace

#endif //__SCRIPT_TEST_HEADER__