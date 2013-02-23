/* 
 * File:   script_functions.h
 * Author: hammy
 *
 * Created on September 25, 2012, 5:22 PM
 */

#ifndef __HGE_SCRIPT_FUNCTIONS_H__
#define	__HGE_SCRIPT_FUNCTIONS_H__

#include <fstream>
#include "script_base.h"

///////////////////////////////////////////////////////////////////////////////
//		Function Class Template Spectializations
///////////////////////////////////////////////////////////////////////////////
/*
 * Numerical Ops
 */
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
