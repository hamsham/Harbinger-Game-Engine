/* 
 * File:   script_functions.cpp
 * Author: hammy
 * 
 * Created on September 25, 2012, 5:22 PM
 */

#include <cmath>
#include "scripting/script_functions.h"

//-----------------------------------------------------------------------------
//		Numerical Operations
//-----------------------------------------------------------------------------
#ifndef MATH_OP
	
	#define MATH_OP( fnc, op, varType, varEnum )\
	template <>\
	void fnc::run() {\
		if ( args[0] == nullptr || args[1] == nullptr || retVal == nullptr ) return;\
		HGE_ASSERT (\
			(args[0]->getScriptSubType() == varEnum) &&\
			(args[1]->getScriptSubType() == varEnum) &&\
			(retVal->getScriptSubType() == varEnum)\
		);\
		varType* pRet = reinterpret_cast< varType* >( retVal );\
		varType* pArgA = reinterpret_cast< varType* >( args[0] );\
		varType* pArgB = reinterpret_cast< varType* >( args[1] );\
		pRet->data = pArgA->data op pArgB->data;\
	}

	MATH_OP( c_fncIntAdd, +, c_varInt, SCRIPT_VAR_INT )
	MATH_OP( c_fncIntSub, -, c_varInt, SCRIPT_VAR_INT )
	MATH_OP( c_fncIntMul, *, c_varInt, SCRIPT_VAR_INT )
	MATH_OP( c_fncIntDiv, /, c_varInt, SCRIPT_VAR_INT )
	MATH_OP( c_fncIntMod, %, c_varInt, SCRIPT_VAR_INT )
	MATH_OP( c_fncIntEql, =, c_varInt, SCRIPT_VAR_INT )

	MATH_OP( c_fncFloatAdd, +, c_varFloat, SCRIPT_VAR_FLOAT )
	MATH_OP( c_fncFloatSub, -, c_varFloat, SCRIPT_VAR_FLOAT )
	MATH_OP( c_fncFloatMul, *, c_varFloat, SCRIPT_VAR_FLOAT )
	MATH_OP( c_fncFloatDiv, /, c_varFloat, SCRIPT_VAR_FLOAT )
	MATH_OP( c_fncFloatEql, =, c_varFloat, SCRIPT_VAR_FLOAT )

	#undef MATH_OP
#endif

//-----------------------------------------------------------------------------
// Floating Point Modulus
//-----------------------------------------------------------------------------
template <>
void c_fncFloatMod::run() {
	if ( args[0] == nullptr || args[1] == nullptr || retVal == nullptr ) return;
	
	HGE_ASSERT(
		(args[0]->getScriptSubType() == SCRIPT_VAR_FLOAT) &&
		(args[1]->getScriptSubType() == SCRIPT_VAR_FLOAT) &&
		(retVal->getScriptSubType() == SCRIPT_VAR_FLOAT)
	);
	
	c_varFloat* pRet = reinterpret_cast< c_varFloat* >( retVal );
	c_varFloat* pArgA = reinterpret_cast< c_varFloat* >( args[0] );
	c_varFloat* pArgB = reinterpret_cast< c_varFloat* >( args[1] );
	
	pRet->data = fmod( pArgA->data, pArgB->data );
}

//-----------------------------------------------------------------------------
// Type-Casting from Float to Int
//-----------------------------------------------------------------------------
template <>
void c_fncIntCast::run() {
	if ( args[0] == nullptr ) return;
	
	HGE_ASSERT(
		(args[0]->getScriptSubType() == SCRIPT_VAR_FLOAT) &&
		(retVal->getScriptSubType() == SCRIPT_VAR_INT)
	);
	
	c_varInt* pRet = reinterpret_cast< c_varInt* >( retVal );
	c_varFloat* pArgA = reinterpret_cast< c_varFloat* >( args[0] );
	
	pRet->data = static_cast< int >( pArgA->data );
}

//-----------------------------------------------------------------------------
// Type-Casting from Int to Float
//-----------------------------------------------------------------------------
template <>
void c_fncFloatCast::run() {
	if ( args[0] == nullptr ) return;
	
	HGE_ASSERT(
		(args[0]->getScriptSubType() == SCRIPT_VAR_INT) &&
		(retVal->getScriptSubType() == SCRIPT_VAR_FLOAT)
	);
	
	c_varFloat* pRet = reinterpret_cast< c_varFloat* >( retVal );
	c_varInt* pArgA = reinterpret_cast< c_varInt* >( args[0] );
	
	pRet->data = static_cast< float >( pArgA->data );
}

//-----------------------------------------------------------------------------
// Floating Poiint Rounding
//-----------------------------------------------------------------------------
template <>
void c_fncNumRound::run() {
	if ( args[0] == nullptr ) return;
	
	HGE_ASSERT(
		(args[0]->getScriptSubType() == SCRIPT_VAR_FLOAT) &&
		(retVal->getScriptSubType() == SCRIPT_VAR_FLOAT)
	);
	
	c_varFloat* pRet = reinterpret_cast< c_varFloat* >( retVal );
	c_varFloat* pArgA = reinterpret_cast< c_varFloat* >( args[0] );
	
	pRet->data = std::floor( pArgA->data + 0.5f );
}
