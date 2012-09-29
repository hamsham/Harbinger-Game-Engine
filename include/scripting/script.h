
//Scripting prototype (again)
#ifndef __SCRIPT_TEST_HEADER__
#define __SCRIPT_TEST_HEADER__

namespace harbinger {

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

#include "script_base.h"
#include "script_variables.h"
#include "script_functions.h"

#endif //__SCRIPT_TEST_HEADER__