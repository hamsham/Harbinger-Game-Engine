
//Scripting prototype (again)
#ifndef __SCRIPT_TEST_HEADER__
#define __SCRIPT_TEST_HEADER__

namespace harbinger {

//-----------------------------------------------------------------------------
//		Script Types
//-----------------------------------------------------------------------------
//DO NOT USE AS BIT-FLAGS! These are used for versioning,
enum e_scriptType { 
	//base classes
	SCRIPT_BASE				= 0x00001,
	SCRIPT_VAR				= 0x00002,
	SCRIPT_FUNC				= 0x00004,
	
	//functions
	SCRIPT_FUNC_EVAL			= 0x00010,
	SCRIPT_FUNC_NUM_EVAL		= 0x00020,
	SCRIPT_FUNC_NUM_ARITHMETIC	= 0x00040,
	SCRIPT_FUNC_NUM_MATH		= 0x00080,
	SCRIPT_FUNC_NUM_TRIG		= 0x00100,
	
	//variables
	SCRIPT_VAR_NUM				= 0x00200,
	SCRIPT_VAR_INT				= 0x00400,
	SCRIPT_VAR_UINT			= 0x00800,
	SCRIPT_VAR_FLOAT			= 0x01000,
	SCRIPT_VAR_FIXED			= 0x02000,
	SCRIPT_VAR_VEC_2D			= 0x04000,
	SCRIPT_VAR_VEC_3D			= 0x08000,
	SCRIPT_VAR_STRING			= 0x10000,
	SCRIPT_VAR_BOOL			= 0x20000,

	SCRIPT_INVALID				= 0
};

//-----------------------------------------------------------------------------
//		Forward Declarations
//-----------------------------------------------------------------------------
//base class
class c_script;

//function base class
template <typename returnType>
class c_scriptFunc; //abstract,  inherits c_script

//variables
template <typename type>
class c_scriptVar; //inherits c_script
class c_scriptNum; //empty base class, inherits nothing

//numeric variables, all inherit c_scriptVar & c_scriptNum
class c_scriptInt;
class c_scriptUint;
class c_scriptFloat;

//math functions, all inherit c_scriptFunc in some way
class c_scriptEvaluation; //abstract, inherits c_scriptFunction, has a boolean result

class c_scriptNumEval; //inherits c_scriptEval, has a boolean result
class c_scriptMiscMath; //inherits c_scriptFunc. has a float result
class c_scriptArithmetic; //inherits c_scriptFunc. has a c_scriptNum result. takes two arguments (except for the EQL function, which has one)
class c_scriptTrigonometry; //inherits c_scriptFunc. has a float result in radians. takes one argument

} //end harbinger namespace

#endif //__SCRIPT_TEST_HEADER__