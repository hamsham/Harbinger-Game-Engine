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
c_script::~c_script() {}

c_script& c_script::operator= ( const c_script& scriptCopy ) {
	assert( scriptCopy.getScriptSubType() != SCRIPT_INVALID );
	return *this;
}

bool c_script::operator== ( const c_script& scriptCopy ) const {
	return (this == &scriptCopy);
}

bool c_script::operator!= ( const c_script& scriptCopy ) const {
	return (this != &scriptCopy);
}

//-----------------------------------------------------------------------------
//		Variable Base Class
//-----------------------------------------------------------------------------
c_scriptVarBase::~c_scriptVarBase() {}

c_scriptVarBase& c_scriptVarBase::operator = (const c_scriptVarBase& varCopy) {
	assert( varCopy.getScriptSubType() != SCRIPT_INVALID );
	return *this;
}

bool c_scriptVarBase::operator == (const c_scriptVarBase& varCopy) const {
	return (this == &varCopy);
}

bool c_scriptVarBase::operator != (const c_scriptVarBase& varCopy) const {
	return (this != &varCopy);
}

//-----------------------------------------------------------------------------
//		Number Variable Base Class
//-----------------------------------------------------------------------------
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
	assert( inVar.getScriptType() != SCRIPT_INVALID );
	return *this;
}
c_scriptNum& c_scriptNum::operator += ( const c_scriptNum& inVar ) {
	*this = ( *this + inVar );
	return *this;
}
c_scriptNum& c_scriptNum::operator -= ( const c_scriptNum& inVar ) {
	*this = ( *this - inVar );
	return *this;
}
c_scriptNum& c_scriptNum::operator *= ( const c_scriptNum& inVar ) {
	*this = ( *this * inVar );
	return *this;
}
c_scriptNum& c_scriptNum::operator /= ( const c_scriptNum& inVar ) {
	*this = ( *this / inVar );
	return *this;
}
c_scriptNum& c_scriptNum::operator %= ( const c_scriptNum& inVar ) {
	*this = ( *this % inVar );
	return *this;
}

bool c_scriptNum::operator == ( const c_scriptNum& inVar ) const {
	return this == &inVar;
}

bool c_scriptNum::operator != ( const c_scriptNum& inVar ) const {
	return this != &inVar;
}

bool c_scriptNum::operator > ( const c_scriptNum& inVar ) const {
	return this > &inVar;
}

bool c_scriptNum::operator < ( const c_scriptNum& inVar ) const {
	return this < &inVar;
}

bool c_scriptNum::operator >= ( const c_scriptNum& inVar ) const {
	return this >= &inVar;
}

bool c_scriptNum::operator <= ( const c_scriptNum& inVar ) const {
	return this <= &inVar;
}

c_scriptNum& c_scriptNum::operator= ( int inVar ) {
	++inVar;
	return *this;
}

c_scriptNum& c_scriptNum::operator= ( unsigned int inVar ) {
	++inVar;
	return *this;
}

c_scriptNum& c_scriptNum::operator= ( float inVar ) {
	++inVar;
	return *this;
}

c_scriptNum::operator int() const {
	return 0;
}

c_scriptNum::operator unsigned int() const {
	return 0;
}

c_scriptNum::operator float() const {
	return 0.f;
}

//-----------------------------------------------------------------------------
//		Function Base Class
//-----------------------------------------------------------------------------
c_scriptFuncBase::c_scriptFuncBase() :
	nextFunc( NULL )
{}

c_scriptFuncBase::c_scriptFuncBase( const c_scriptFuncBase& funcCopy ) :
	nextFunc( funcCopy.nextFunc )
{}

c_scriptFuncBase::~c_scriptFuncBase() {}

const c_scriptFuncBase* c_scriptFuncBase::getNextFunction() const {
	return nextFunc;
}

void c_scriptFuncBase::setNextFunction(const c_scriptFuncBase& next) {
	nextFunc = &next;
}

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

} // end harbinger namespace
