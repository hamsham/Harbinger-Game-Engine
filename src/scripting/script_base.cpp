/* 
 * File:   script_base.cpp
 * Author: hammy
 * 
 * Created on September 25, 2012, 5:18 PM
 */

#include "../../include/harbinger.h"

namespace harbinger {

//-----------------------------------------------------------------------------
//		Script Base Class
//-----------------------------------------------------------------------------
c_script::c_script() {}

c_script::c_script( const c_script& scriptCopy ) :
	name (scriptCopy.name),
	group (scriptCopy.group)
{}

c_script::~c_script() {}

c_script& c_script::operator= ( const c_script& scriptCopy ) {
	name = scriptCopy.name;
	group = scriptCopy.group;
	return *this;
}

const std::string& c_script::scriptName() {
	return name;
}

const std::string& c_script::scriptGroup() {
	return group;
}

//-----------------------------------------------------------------------------
//		Number Variable Base Class
//-----------------------------------------------------------------------------
c_scriptNum::c_scriptNum() {}
c_scriptNum::c_scriptNum( const c_scriptNum& numCopy ) {}
c_scriptNum::~c_scriptNum() {}

c_scriptNum c_scriptNum::operator + ( const c_scriptNum& inVar ) {
	return inVar;
}
c_scriptNum c_scriptNum::operator - ( const c_scriptNum& inVar ) {
	return inVar;
}
c_scriptNum c_scriptNum::operator * ( const c_scriptNum& inVar ) {
	return inVar;
}
c_scriptNum c_scriptNum::operator / ( const c_scriptNum& inVar ) {
	return inVar;
}
c_scriptNum c_scriptNum::operator % ( const c_scriptNum& inVar ) {
	return inVar;
}

c_scriptNum& c_scriptNum::operator = ( const c_scriptNum& inVar ) {
	return *this;
}
c_scriptNum& c_scriptNum::operator += ( const c_scriptNum& inVar ) {
	return *this;
}
c_scriptNum& c_scriptNum::operator -= ( const c_scriptNum& inVar ) {
	return *this;
}
c_scriptNum& c_scriptNum::operator *= ( const c_scriptNum& inVar ) {
	return *this;
}
c_scriptNum& c_scriptNum::operator /= ( const c_scriptNum& inVar ) {
	return *this;
}
c_scriptNum& c_scriptNum::operator %= ( const c_scriptNum& inVar ) {
	return *this;
}

bool c_scriptNum::operator == ( const c_scriptNum& inVar ) {
	return false;
}

bool c_scriptNum::operator != ( const c_scriptNum& inVar ) {
	return false;
}

bool c_scriptNum::operator > ( const c_scriptNum& inVar ) {
	return false;
}

bool c_scriptNum::operator < ( const c_scriptNum& inVar ) {
	return false;
}

bool c_scriptNum::operator >= ( const c_scriptNum& inVar ) {
	return false;
}

bool c_scriptNum::operator <= ( const c_scriptNum& inVar ) {
	return false;
}

c_scriptNum& c_scriptNum::operator= ( int inVar ) {
	return *this;
}

c_scriptNum& c_scriptNum::operator= ( unsigned int inVar ) {
	return *this;
}

c_scriptNum& c_scriptNum::operator= ( float inVar ) {
	return *this;
}

c_scriptNum::operator int() const {
	return 0;
}

c_scriptNum::operator unsigned int() const {
	return 0;
}

c_scriptNum::operator float() const {
	return 0.0f;
}

//-----------------------------------------------------------------------------
//		Evaluation Function Base Class
//-----------------------------------------------------------------------------
c_scriptEvaluation::c_scriptEvaluation() {
	returnVal = false;
}

c_scriptEvaluation::c_scriptEvaluation( const c_scriptEvaluation& evalCopy ) :
	c_script( evalCopy )
{}

c_scriptEvaluation::c_scriptEvaluation( bool inValue ) {
	
	returnVal = inValue;
}

c_scriptEvaluation::~c_scriptEvaluation() {}

const bool& c_scriptEvaluation::evalResult() const {
	return returnVal;
}

} // end harbinger namespace
