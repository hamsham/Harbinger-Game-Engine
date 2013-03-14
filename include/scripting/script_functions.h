/* 
 * File:   script_functions.h
 * Author: hammy
 *
 * Created on September 25, 2012, 5:22 PM
 */

#ifndef __HGE_SCRIPT_FUNCTIONS_H__
#define	__HGE_SCRIPT_FUNCTIONS_H__

#include "script.h"
#include "script_base.h"
#include "script_variables.h"

///////////////////////////////////////////////////////////////////////////////
//		Convenience Macro
///////////////////////////////////////////////////////////////////////////////
#define REGISTER_SCRIPT_FUNCTION( func_hash, numArgs, func_name )\
typedef c_func< func_hash, numArgs > func_name;\
template <> void HGE_API func_name::run()

///////////////////////////////////////////////////////////////////////////////
//		Typedefs
///////////////////////////////////////////////////////////////////////////////
// Integer Functions
REGISTER_SCRIPT_FUNCTION( SCRIPT_FUNC_INT_ADD, 2, c_fncIntAdd );
REGISTER_SCRIPT_FUNCTION( SCRIPT_FUNC_INT_SUB, 2, c_fncIntSub );
REGISTER_SCRIPT_FUNCTION( SCRIPT_FUNC_INT_MUL, 2, c_fncIntMul );
REGISTER_SCRIPT_FUNCTION( SCRIPT_FUNC_INT_DIV, 2, c_fncIntDiv );
REGISTER_SCRIPT_FUNCTION( SCRIPT_FUNC_INT_MOD, 2, c_fncIntMod );
REGISTER_SCRIPT_FUNCTION( SCRIPT_FUNC_INT_EQL, 2, c_fncIntEql );

// Float Functions
REGISTER_SCRIPT_FUNCTION( SCRIPT_FUNC_FLOAT_ADD, 2, c_fncFloatAdd );
REGISTER_SCRIPT_FUNCTION( SCRIPT_FUNC_FLOAT_SUB, 2, c_fncFloatSub );
REGISTER_SCRIPT_FUNCTION( SCRIPT_FUNC_FLOAT_MUL, 2, c_fncFloatMul );
REGISTER_SCRIPT_FUNCTION( SCRIPT_FUNC_FLOAT_DIV, 2, c_fncFloatDiv );
REGISTER_SCRIPT_FUNCTION( SCRIPT_FUNC_FLOAT_MOD, 2, c_fncFloatMod );
REGISTER_SCRIPT_FUNCTION( SCRIPT_FUNC_FLOAT_EQL, 2, c_fncFloatEql );

// Type-Casting
REGISTER_SCRIPT_FUNCTION( SCRIPT_FUNC_INT_CAST, 1, c_fncIntCast );
REGISTER_SCRIPT_FUNCTION( SCRIPT_FUNC_FLOAT_CAST, 1, c_fncFloatCast );

// Misc Numerical Ops
REGISTER_SCRIPT_FUNCTION( SCRIPT_FUNC_NUM_ROUND, 1, c_fncNumRound );

#endif	/* __HGE_SCRIPT_FUNCTIONS_H__ */
