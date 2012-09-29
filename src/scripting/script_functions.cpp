/* 
 * File:   script_functions.cpp
 * Author: hammy
 * 
 * Created on September 25, 2012, 5:22 PM
 */

#include "../../include/harbinger.h"

namespace harbinger {

//-----------------------------------------------------------------------------
//		Numerical Evaluations
//-----------------------------------------------------------------------------
c_scriptNumEval::c_scriptNumEval() :
	evalType( c_scriptNumEval::IS_EQUAL ),
	evalNum( NULL ),
	refNum( NULL )
{}

c_scriptNumEval::c_scriptNumEval( const c_scriptNumEval& evalCopy ) :
	c_scriptEvaluation( evalCopy ),
	evalType( evalCopy.evalType ),
	evalNum( evalCopy.evalNum ),
	refNum( evalCopy.refNum )
{}

c_scriptNumEval::~c_scriptNumEval(){}

const c_scriptNum* c_scriptNumEval::getNumToEvaluate() {
	return evalNum;
}

const c_scriptNum* c_scriptNumEval::getNumToReference() {
	return refNum;
}

void c_scriptNumEval::setNumToEvaluate( c_scriptNum* arg ) {
	evalNum = arg;
}

void c_scriptNumEval::setNumToReference( c_scriptNum* arg ) {
	refNum = arg;
}

void c_scriptNumEval::setEvalType( e_evalType eval ) {
	evalType = eval;
}

void c_scriptNumEval::setEvalType( int eval ) {
	evalType = eval;
}

int c_scriptNumEval::getEvalType() const {
	return evalType;
}

void c_scriptNumEval::run() {
	//ensure that there are numbers to evaluate
	if (evalNum == NULL || refNum == NULL) {
		returnVal = false;
		return;
	}
	
	switch ( evalType ) {
		case IS_EQUAL:
			returnVal = (*evalNum == *refNum);
			break;
		case IS_NOT_EQUAL:
			returnVal = (*evalNum != *refNum);
			break;
		case IS_GREATER:
			returnVal = (*evalNum > *refNum);
			break;
		case IS_LESS:
			returnVal = (*evalNum < *refNum);
			break;
		case IS_GREATER_OR_EQUAL:
			returnVal = (*evalNum >= *refNum);
			break;
		case IS_LESS_OR_EQUAL:
			returnVal = (*evalNum <= *refNum);
			break;
		default:
			returnVal = false;
	}
}

void c_scriptNumEval::tick(float timeElapsed) {
	run();
}

//-----------------------------------------------------------------------------
//		Misc. Mathematical Functions
//-----------------------------------------------------------------------------
c_scriptMiscMath::c_scriptMiscMath() :
	evalType( c_scriptMiscMath::SQRT ),
	evalNum( NULL )
{}

c_scriptMiscMath::c_scriptMiscMath( const c_scriptMiscMath& evalCopy ) :
	c_scriptFunc( evalCopy ),
	evalType( evalCopy.evalType ),
	evalNum( evalCopy.evalNum )
{}

c_scriptMiscMath::~c_scriptMiscMath() {}

const c_scriptNum* c_scriptMiscMath::getNumToEvaluate() {
	return evalNum;
}

void c_scriptMiscMath::setNumToEvaluate( c_scriptNum* arg ) {
	evalNum = arg;
}

void c_scriptMiscMath::setEvalType( c_scriptMiscMath::e_math eval ) {
	evalType = eval;
}

void c_scriptMiscMath::setEvalType( int eval ) {
	evalType = eval;
}

int c_scriptMiscMath::getEvalType() const {
	return evalType;
}

void c_scriptMiscMath::run() {
	//ensure that there are numbers to evaluate
	if (evalNum == NULL) {
		returnVal = 0.0f;
		return;
	}
	
	switch ( evalType ) {
		case SQRT:
			returnVal = std::sqrt( (float)(*evalNum) );
			break;
		case LOG:
			returnVal = std::log( (float)(*evalNum) );
			break;
		case ABS:
			returnVal = std::fabs( (float)(*evalNum) );
			break;
		case RND:
			returnVal = std::floor( (float)(*evalNum) + 0.5f );
			break;
		case CEIL:
			returnVal = std::ceil( (float)(*evalNum) );
			break;
		case FLOOR:
			returnVal = std::floor( (float)(*evalNum) );
			break;
		default:
			returnVal = 0.0f;
	}
}
void c_scriptMiscMath::tick( float timeElapsed ) {
	run();
}

//-----------------------------------------------------------------------------
//		Numerical Math/Arithmetic
//-----------------------------------------------------------------------------
c_scriptArithmetic::c_scriptArithmetic() :
	evalType( c_scriptArithmetic::ADD ),
	evalNum( NULL ),
	refNum( NULL )
{
	returnVal = 0.0f;
}

c_scriptArithmetic::c_scriptArithmetic( const c_scriptArithmetic& evalCopy ) :
	c_scriptFunc( evalCopy ),
	evalType( evalCopy.evalType ),
	evalNum( evalCopy.evalNum ),
	refNum( evalCopy.refNum )
{}

c_scriptArithmetic::~c_scriptArithmetic(){}

const c_scriptNum* c_scriptArithmetic::getNumToEvaluate() {
	return evalNum;
}

const c_scriptNum* c_scriptArithmetic::getNumToReference() {
	return refNum;
}

void c_scriptArithmetic::setNumToEvaluate( c_scriptNum* arg ) {
	evalNum = arg;
}

void c_scriptArithmetic::setNumToReference( c_scriptNum* arg ) {
	refNum = arg;
}

void c_scriptArithmetic::setEvalType( e_arithmetic eval ) {
	evalType = eval;
}

void c_scriptArithmetic::setEvalType( int eval ) {
	evalType = eval;
}

int c_scriptArithmetic::getEvalType() const {
	return evalType;
}

void c_scriptArithmetic::run() {
	//ensure that there are numbers to evaluate
	if (evalNum == NULL || refNum == NULL) {
		returnVal = 0;
		return;
	}
	
	switch ( evalType ) {
		case ADD:
			returnVal = (*evalNum + *refNum);
			break;
		case SUB:
			returnVal = (*evalNum - *refNum);
			break;
		case MUL:
			returnVal = (*evalNum * *refNum);
			break;
		case DIV:
			returnVal = (*evalNum / *refNum);
			break;
		case MOD:
			returnVal = (*evalNum % *refNum);
			break;
		case POW:
			returnVal = std::pow( (float)(*evalNum), (float)(*refNum) );
			break;
		case EQL:
			returnVal = *refNum;
			break;
		default:
			returnVal = 0;
	}
}

void c_scriptArithmetic::tick(float timeElapsed) {
	run();
}

//-----------------------------------------------------------------------------
//		Trigonometric Functinos
//-----------------------------------------------------------------------------
c_scriptTrigonometry::c_scriptTrigonometry() :
	evalType( c_scriptTrigonometry::SIN ),
	evalNum( NULL )
{
	returnVal = 0.0f;
}

c_scriptTrigonometry::c_scriptTrigonometry( const c_scriptTrigonometry& trigCopy ) :
	c_scriptFunc( trigCopy ),
	evalType( trigCopy.evalType ),
	evalNum( trigCopy.evalNum )
{}

c_scriptTrigonometry::~c_scriptTrigonometry(){}

const c_scriptNum* c_scriptTrigonometry::getNumToEvaluate() {
	return evalNum;
}

void c_scriptTrigonometry::setNumToEvaluate( c_scriptNum* arg ) {
	evalNum = arg;
}

void c_scriptTrigonometry::setEvalType( e_trigonometry eval ) {
	evalType = eval;
}

void c_scriptTrigonometry::setEvalType( int eval ) {
	evalType = eval;
}

int c_scriptTrigonometry::getEvalType() const {
	return evalType;
}

void c_scriptTrigonometry::run() {
	if (evalNum == NULL) {
		returnVal = 0;
		return;
	}
	float divZeroCheck( 0 );
	
	switch ( evalType ) {
		case SIN:
			returnVal = std::sin( (float)(*evalNum) );
			break;
		case COS:
			returnVal = std::cos( (float)(*evalNum) );
			break;
		case TAN:
			returnVal = std::tan( (float)(*evalNum) );
			break;
		case CSC:
			divZeroCheck = std::sin( (float)(*evalNum) );	//runtime error checking
			returnVal = (divZeroCheck != 0) ? 1.0f/divZeroCheck : 0.0f; //never divide by 0!
			break;
		case SEC:
			divZeroCheck = std::cos( (float)(*evalNum) );
			returnVal = (divZeroCheck != 0) ? 1.0f/divZeroCheck : 0.0f;
			break;
		case COT:
			divZeroCheck = std::cos( (float)(*evalNum) );
			returnVal = (divZeroCheck != 0) ? 1.0f/divZeroCheck : 0.0f;
			break;
		case ARC_SIN:
			returnVal = std::asin( (float)(*evalNum) );
			break;
		case ARC_COS:
			returnVal = std::acos( (float)(*evalNum) );
			break;
		case ARC_TAN:
			returnVal = std::atan( (float)(*evalNum) );
			break;
		case HYP_SIN:
			returnVal = std::sinh( (float)(*evalNum) );
			break;
		case HYP_COS:
			returnVal = std::cosh( (float)(*evalNum) );
			break;
		case HYP_TAN:
			returnVal = std::tanh( (float)(*evalNum) );
			break;
		default:
			returnVal = 0;
	}
}

void c_scriptTrigonometry::tick(float timeElapsed) {
	run();
}

} // end harbinger namespace
