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
//		Typedefs
///////////////////////////////////////////////////////////////////////////////
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

///////////////////////////////////////////////////////////////////////////////
//		Function Class Specializations
///////////////////////////////////////////////////////////////////////////////
// Numerical Operations
template <> void HGE_API c_fncIntAdd::run();
template <> void HGE_API c_fncIntSub::run();
template <> void HGE_API c_fncIntMul::run();
template <> void HGE_API c_fncIntDiv::run();
template <> void HGE_API c_fncIntMod::run();
template <> void HGE_API c_fncIntEql::run();

template <> void HGE_API c_fncFloatAdd::run();
template <> void HGE_API c_fncFloatSub::run();
template <> void HGE_API c_fncFloatMul::run();
template <> void HGE_API c_fncFloatDiv::run();
template <> void HGE_API c_fncFloatMod::run();
template <> void HGE_API c_fncFloatEql::run();

template <> void HGE_API c_fncIntCast::run();
template <> void HGE_API c_fncFloatCast::run();

template <> void HGE_API c_fncNumRound::run();

#endif	/* __HGE_SCRIPT_FUNCTIONS_H__ */
