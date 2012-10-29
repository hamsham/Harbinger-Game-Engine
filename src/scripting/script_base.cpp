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
const char* c_script::defaultName = "???";
const char* c_script::invalidName = "NULL";

c_script::c_script() :
	name( defaultName )
{}

c_script::c_script( const c_script& scriptCopy ) :
	name ( scriptCopy.name )
{}

c_script::~c_script() {}

c_script& c_script::operator= ( const c_script& scriptCopy ) {
	name = scriptCopy.name;
	return *this;
}

bool c_script::operator== ( const c_script& scriptCopy ) const {
	return name == scriptCopy.name;
}

bool c_script::operator!= ( const c_script& scriptCopy ) const {
	return name != scriptCopy.name;
}

const std::string& c_script::getName() const {
	return name;
}

void c_script::setName( const std::string& inName ) {
	if ( inName.size() > 0 && inName != invalidName ) 
		name = inName;
	else
		name = defaultName;
}

//-----------------------------------------------------------------------------
//		Variable Base Class
//-----------------------------------------------------------------------------
c_scriptVarBase& c_scriptVarBase::operator = (const c_scriptVarBase& varCopy) {
	name = varCopy.name;
	return *this;
}

bool c_scriptVarBase::operator == (const c_scriptVarBase& varCopy) const {
	return (name == varCopy.name);
}

bool c_scriptVarBase::operator != (const c_scriptVarBase& varCopy) const {
	return (name != varCopy.name);
}

//-----------------------------------------------------------------------------
//		Number Variable Base Class
//-----------------------------------------------------------------------------
c_scriptNum::c_scriptNum() {}
c_scriptNum::c_scriptNum( const c_scriptNum& numCopy ) {}
c_scriptNum::~c_scriptNum() {}

c_scriptNum c_scriptNum::operator + ( const c_scriptNum& inVar ) const {
	return inVar;
}
c_scriptNum c_scriptNum::operator - ( const c_scriptNum& inVar ) const {
	return inVar;
}
c_scriptNum c_scriptNum::operator * ( const c_scriptNum& inVar ) const {
	return inVar;
}
c_scriptNum c_scriptNum::operator / ( const c_scriptNum& inVar ) const {
	return inVar;
}
c_scriptNum c_scriptNum::operator % ( const c_scriptNum& inVar ) const {
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

bool c_scriptNum::operator == ( const c_scriptNum& inVar ) const {
	return false;
}

bool c_scriptNum::operator != ( const c_scriptNum& inVar ) const {
	return false;
}

bool c_scriptNum::operator > ( const c_scriptNum& inVar ) const {
	return false;
}

bool c_scriptNum::operator < ( const c_scriptNum& inVar ) const {
	return false;
}

bool c_scriptNum::operator >= ( const c_scriptNum& inVar ) const {
	return false;
}

bool c_scriptNum::operator <= ( const c_scriptNum& inVar ) const {
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
//		Function Base Class
//-----------------------------------------------------------------------------
c_scriptFuncBase::c_scriptFuncBase() {}

c_scriptFuncBase::c_scriptFuncBase( const c_scriptFuncBase& funcCopy ) :
	c_script( funcCopy )
{}

c_scriptFuncBase::~c_scriptFuncBase() {}

//-----------------------------------------------------------------------------
//		Function Class (Template Specializations)
//-----------------------------------------------------------------------------
template <>
c_scriptFunc< void* >::c_scriptFunc() :
	returnVal( NULL )
{}

template <>
c_scriptFunc< void* >::c_scriptFunc( const c_scriptFunc& funcCopy ) :
	c_scriptFuncBase( funcCopy ),
	returnVal( NULL )
{}

//-----------------------------------------------------------------------------
//		Evaluation Function Base Class
//-----------------------------------------------------------------------------
c_scriptEvaluation::c_scriptEvaluation() :
	evalType( 0 )
{
	returnVal = false;
}

c_scriptEvaluation::c_scriptEvaluation( const c_scriptEvaluation& evalCopy ) :
	c_scriptFunc( evalCopy ),
	evalType( evalCopy.evalType )
{
	returnVal = false;
}

c_scriptEvaluation::~c_scriptEvaluation() {}

int c_scriptEvaluation::getEvalType() const {
	return evalType;
}

void c_scriptEvaluation::setEvalType( int eval ) {
	evalType = eval;
}

const c_scriptVarBase* c_scriptEvaluation::getVarToEvaluate() const {
	return NULL;
}

const c_scriptVarBase* c_scriptEvaluation::getVarToReference() const {
	return NULL;
}

} // end harbinger namespace
