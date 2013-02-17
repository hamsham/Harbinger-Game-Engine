/* 
 * File:   script_functions.cpp
 * Author: hammy
 * 
 * Created on September 25, 2012, 5:22 PM
 */

#include <cmath>
#include "scripting/script.h"
#include "scripting/script_functions.h"

//-----------------------------------------------------------------------------
//		Evaluation Function Base Class
//-----------------------------------------------------------------------------
c_scriptEvaluation::c_scriptEvaluation() :
	c_scriptFunc( false ),
	evalType( 0 ),
	evalVar( HGE_NULL ),
	compVar( HGE_NULL )
{}

c_scriptEvaluation::c_scriptEvaluation( const c_scriptEvaluation& evalCopy ) :
	c_scriptFuncBase( evalCopy ),
	c_scriptFunc( evalCopy ),
	evalType( evalCopy.evalType ),
	evalVar( evalCopy.evalVar ),
	compVar( evalCopy.compVar )
{}

c_scriptEvaluation::~c_scriptEvaluation() {}

//file input
void c_scriptEvaluation::read( std::ifstream& fin, scriptMap_t& scrMap ) {
	ulong evalPtr( 0 ), compPtr( 0 );
	bool retVal( false );
	c_scriptFuncBase::read( fin, scrMap );
	fin >> retVal >> evalPtr >> compPtr;
	returnVal = retVal;
	evalVar = dynamic_cast< const c_scriptVarBase* >( scrMap[ evalPtr ] );
	compVar = dynamic_cast< const c_scriptVarBase* >( scrMap[ compPtr ] );
}

//file output
void c_scriptEvaluation::write( std::ofstream& fout ) const {
	c_scriptFuncBase::write( fout );
	fout
		<< " " << (bool)returnVal
		<< " " << evalVar
		<< " " << compVar;
}

int c_scriptEvaluation::getEvalType() const {
	return evalType;
}

void c_scriptEvaluation::setEvalType( int eval ) {
	evalType = eval;
}

const c_scriptVarBase* c_scriptEvaluation::getVarToEvaluate() const {
	return evalVar;
}

void c_scriptEvaluation::setVarToEvaluate( const c_scriptVarBase* inVar ) {
	evalVar = inVar;
}

const c_scriptVarBase* c_scriptEvaluation::getVarToCompare() const {
	return compVar;
}

void c_scriptEvaluation::setVarToCompare( const c_scriptVarBase* inVar ) {
	compVar = inVar;
}

//-----------------------------------------------------------------------------
//		Numerical Function Base Class
//-----------------------------------------------------------------------------
c_scriptNumeric::c_scriptNumeric() :
	c_scriptFunc( 0.f ),
	evalType( 0 ),
	evalVar( HGE_NULL ),
	compVar( HGE_NULL )
{}

c_scriptNumeric::c_scriptNumeric( const c_scriptNumeric& numFunc ) :
	c_scriptFuncBase( numFunc ),
	c_scriptFunc( numFunc ),
	evalType( numFunc.evalType ),
	evalVar( numFunc.evalVar ),
	compVar( numFunc.compVar )
{}

c_scriptNumeric::~c_scriptNumeric() {}

//file input
void c_scriptNumeric::read( std::ifstream& fin, scriptMap_t& scrMap ) {
	ulong evalPtr( 0 ), compPtr( 0 );
	float retVal( 0.f );
	c_scriptFuncBase::read( fin, scrMap );
	fin >> evalType >> retVal >> evalPtr >> compPtr;
	returnVal = retVal;
	evalVar = dynamic_cast< const c_scriptNum* >( scrMap[ evalPtr ] );
	compVar = dynamic_cast< const c_scriptNum* >( scrMap[ compPtr ] );
}

//file output
void c_scriptNumeric::write( std::ofstream& fout ) const {
	c_scriptFuncBase::write( fout );
	fout
		<< " " << evalType
		<< " " << (float)returnVal
		<< " " << evalVar
		<< " " << compVar;
}

int c_scriptNumeric::getEvalType() const {
	return evalType;
}

void c_scriptNumeric::setEvalType( int eval ) {
	evalType = eval;
}

const c_scriptNum* c_scriptNumeric::getVarToEvaluate() const {
	return evalVar;
}

void c_scriptNumeric::setVarToEvaluate( const c_scriptNum* inVar ) {
	evalVar = inVar;
}

const c_scriptNum* c_scriptNumeric::getVarToCompare() const {
	return compVar;
}

void c_scriptNumeric::setVarToCompare( const c_scriptNum* inVar ) {
	compVar = inVar;
}

//-----------------------------------------------------------------------------
//		Numerical Evaluations
//-----------------------------------------------------------------------------
c_scriptNumEval::c_scriptNumEval() {}

c_scriptNumEval::c_scriptNumEval( const c_scriptNumEval& evalCopy ) :
	c_scriptFuncBase( evalCopy ),
	c_scriptFunc( evalCopy ),
	c_scriptEvaluation( evalCopy )
{}

c_scriptNumEval::~c_scriptNumEval(){}

const c_scriptNum* c_scriptNumEval::getVarToEvaluate() const {
	return reinterpret_cast< const c_scriptNum* >( evalVar );
}

void c_scriptNumEval::setVarToEvaluate( const c_scriptNum* inVar ) {
	evalVar = inVar;
}

const c_scriptNum* c_scriptNumEval::getVarToCompare() const {
	return reinterpret_cast< const c_scriptNum* >( compVar );
}

void c_scriptNumEval::setVarToCompare( const c_scriptNum* inVar ) {
	compVar = inVar;
}

void c_scriptNumEval::setEvalType( int eval ) {
	HGE_ASSERT( (evalType >= IS_EQUAL && evalType < FUNC_NUM_INVALID) );
	evalType = eval;
}

void c_scriptNumEval::run() {
	//ensure that there are numbers to evaluate
	if (evalVar == HGE_NULL || compVar == HGE_NULL) {
		returnVal.setFalse();
		return;
	}
	
	const c_scriptNum* pEval = reinterpret_cast< const c_scriptNum* >( evalVar );
	const c_scriptNum* pComp = reinterpret_cast< const c_scriptNum* >( compVar );
	
	switch ( evalType ) {
		case IS_EQUAL:
			returnVal = ( *pEval == *pComp );
			break;
		case IS_NOT_EQUAL:
			returnVal = ( *pEval != *pComp );
			break;
		case IS_GREATER:
			returnVal = ( *pEval > *pComp );
			break;
		case IS_LESS:
			returnVal = ( *pEval < *pComp );
			break;
		case IS_GREATER_OR_EQUAL:
			returnVal = ( *pEval >= *pComp );
			break;
		case IS_LESS_OR_EQUAL:
			returnVal = ( *pEval <= *pComp );
			break;
		default:
			returnVal.setFalse();
	}
}

//-----------------------------------------------------------------------------
//		Misc. Mathematical Functions
//-----------------------------------------------------------------------------
c_scriptMiscMath::c_scriptMiscMath() {}

c_scriptMiscMath::c_scriptMiscMath( const c_scriptMiscMath& evalCopy ) :
	c_scriptFuncBase( evalCopy ),
	c_scriptFunc( evalCopy ),
	c_scriptNumeric( evalCopy )
{}

c_scriptMiscMath::~c_scriptMiscMath() {}

void c_scriptMiscMath::setEvalType( int eval ) {
	HGE_ASSERT( (evalType >= SQRT && evalType < FUNC_MATH_INVALID) );
	evalType = eval;
}

void c_scriptMiscMath::run() {
	//ensure that there are numbers to evaluate
	if (evalVar == HGE_NULL) {
		returnVal = 0.0f;
		return;
	}
	
	//opting for an array of function pointers instead of switch statements
	float ( *mathFunc[] )( float ) = {
		std::sqrt,
		std::log,
		std::fabs,
		std::floor,
		std::ceil
	};
	if ( (evalType >= SQRT) && (evalType < FUNC_MATH_INVALID) ) {
		if ( evalType == RND ) {
			returnVal = mathFunc[ evalType ]( floor(*evalVar) + 0.5f );
		}
		else {
			returnVal = mathFunc[ evalType ]( (float)(*evalVar) );
		}
	}
	else {
		returnVal = 0.f;
	}
}

//-----------------------------------------------------------------------------
//		Numerical Math/Arithmetic
//-----------------------------------------------------------------------------
c_scriptArithmetic::c_scriptArithmetic() {}

c_scriptArithmetic::c_scriptArithmetic( const c_scriptArithmetic& evalCopy ) :
	c_scriptFuncBase( evalCopy ),
	c_scriptFunc( evalCopy ),
	c_scriptNumeric( evalCopy )
{}

c_scriptArithmetic::~c_scriptArithmetic() {}

void c_scriptArithmetic::setEvalType( int eval ) {
	HGE_ASSERT( (evalType >= ADD && evalType < FUNC_ARITH_INVALID) );
	evalType = eval;
}

void c_scriptArithmetic::run() {
	//ensure that there are numbers to evaluate
	if (evalVar == HGE_NULL || compVar == HGE_NULL) {
		returnVal = 0;
		return;
	}
	
	switch ( evalType ) {
		case ADD:
			returnVal = (*evalVar + *compVar);
			break;
		case SUB:
			returnVal = (*evalVar - *compVar);
			break;
		case MUL:
			returnVal = (*evalVar * *compVar);
			break;
		case DIV:
			returnVal = (*evalVar / *compVar);
			break;
		case MOD:
			returnVal = (*evalVar % *compVar);
			break;
		case POW:
			returnVal = std::pow( (float)(*evalVar), (float)(*compVar) );
			break;
		case EQL:
			returnVal = *evalVar;
			break;
		default:
			returnVal = 0.f;
	}
}

//-----------------------------------------------------------------------------
//		Trigonometric Functinos
//-----------------------------------------------------------------------------
c_scriptTrigonometry::c_scriptTrigonometry() {}

c_scriptTrigonometry::c_scriptTrigonometry( const c_scriptTrigonometry& evalCopy ) :
	c_scriptFuncBase( evalCopy ),
	c_scriptFunc( evalCopy ),
	c_scriptNumeric( evalCopy )
{}

c_scriptTrigonometry::~c_scriptTrigonometry() {}

void c_scriptTrigonometry::setEvalType( int eval ) {
	HGE_ASSERT( (evalType >= SIN && evalType < FUNC_TRIG_INVALID) );
	evalType = eval;
}

void c_scriptTrigonometry::run() {
	if (evalVar == HGE_NULL) {
		returnVal = 0.f;
		return;
	}
	
	//opting for an array of function pointers instead of switch statements
	float ( *mathFunc[] )( float ) = {
		std::sin,
		std::cos,
		std::tan,
		std::asin,
		std::acos,
		std::atan,
		std::sinh,
		std::cosh,
		std::tanh,
	};
	
	if ( (evalType >= SIN) && (evalType < FUNC_TRIG_INVALID) ) {
		//check if secant, cosecant, or cotangent functions were requested
		if ( (evalType >= CSC) && (evalType <= COT) ) {
			float divByZeroCheck( mathFunc[ evalType-3 ]( *evalVar ) );
			returnVal = divByZeroCheck ? 1.0f/divByZeroCheck : 0.0f;
		}
		else {
			returnVal = mathFunc[ evalType ]( (float)(*evalVar) );
		}
	}
	else {
		returnVal = 0.f;
	}
	/*
	switch ( evalType ) {
		case SIN:
			returnVal = std::sin( (float)(*evalVar) );
			break;
		case COS:
			returnVal = std::cos( (float)(*evalVar) );
			break;
		case TAN:
			returnVal = std::tan( (float)(*evalVar) );
			break;
		case CSC:
			divZeroCheck = std::sin( (float)(*evalVar) );	//runtime error checking
			returnVal = (divZeroCheck != 0) ? 1.0f/divZeroCheck : 0.0f; //never divide by 0!
			break;
		case SEC:
			divZeroCheck = std::cos( (float)(*evalVar) );
			returnVal = (divZeroCheck != 0) ? 1.0f/divZeroCheck : 0.0f;
			break;
		case COT:
			divZeroCheck = std::tan( (float)(*evalVar) );
			returnVal = (divZeroCheck != 0) ? 1.0f/divZeroCheck : 0.0f;
			break;
		case ARC_SIN:
			returnVal = std::asin( (float)(*evalVar) );
			break;
		case ARC_COS:
			returnVal = std::acos( (float)(*evalVar) );
			break;
		case ARC_TAN:
			returnVal = std::atan( (float)(*evalVar) );
			break;
		case HYP_SIN:
			returnVal = std::sinh( (float)(*evalVar) );
			break;
		case HYP_COS:
			returnVal = std::cosh( (float)(*evalVar) );
			break;
		case HYP_TAN:
			returnVal = std::tanh( (float)(*evalVar) );
			break;
		default:
			returnVal = 0.f;
	}
	*/
}
