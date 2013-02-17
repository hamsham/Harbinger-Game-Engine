/* 
 * File:   script_base.cpp
 * Author: hammy
 * 
 * Created on September 25, 2012, 5:18 PM
 */

#include "math/math.h"
using namespace hamLibs;

#include "scripting/script.h"
#include "scripting/script_base.h"

//-----------------------------------------------------------------------------
//		Script Base Class
//-----------------------------------------------------------------------------
c_script::~c_script() {}

c_script& c_script::operator= ( const c_script& scriptCopy ) {
	HGE_ASSERT( scriptCopy.getScriptSubType() != SCRIPT_INVALID );
	return *this;
}

bool c_script::operator== ( const c_script& scriptCopy ) const {
	return (this == &scriptCopy);
}

bool c_script::operator!= ( const c_script& scriptCopy ) const {
	return (this != &scriptCopy);
}

void c_script::read( std::ifstream& fin, scriptMap_t& scrMap ) {
	ulong varPtr( 0 );
	fin >> varPtr;
	scrMap[ varPtr ] = this;
}

void c_script::write( std::ofstream& fout ) const {
#ifdef __GNUC__
	// As far as I know, only GCC/G++ increments pointers when writing them to files
	fout << (this - sizeof( void* ));
#else
	fout << this;
#endif
}

//-----------------------------------------------------------------------------
//		Variable Base Class
//-----------------------------------------------------------------------------
c_scriptVarBase::~c_scriptVarBase() {}

c_scriptVarBase& c_scriptVarBase::operator = (const c_scriptVarBase& varCopy) {
	HGE_ASSERT( varCopy.getScriptSubType() != SCRIPT_INVALID );
	return *this;
}

bool c_scriptVarBase::operator == (const c_scriptVarBase& varCopy) const {
	return (this == &varCopy);
}

bool c_scriptVarBase::operator != (const c_scriptVarBase& varCopy) const {
	return (this != &varCopy);
}

//-----------------------------------------------------------------------------
//		Variable Interface Classe
//		Abstract
//		All variables MUST support copy construction & assignment operators
//-----------------------------------------------------------------------------
template <>
void c_scriptVar< math::vec3 >::read( std::ifstream& fin, scriptMap_t& scrMap ) {
	c_script::read( fin, scrMap );
	fin >> variable[0] >> variable[1] >> variable[2];
}

template <>
void c_scriptVar< math::vec3 >::write( std::ofstream& fout ) const {
	c_script::write( fout );
	fout << " " << variable[0] << " " << variable[1] << " " << variable[2];
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
	HGE_ASSERT( inVar.getScriptType() != SCRIPT_INVALID );
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
	nextFunc( HGE_NULL )
{}

c_scriptFuncBase::c_scriptFuncBase( const c_scriptFuncBase& funcCopy ) :
	nextFunc( funcCopy.nextFunc )
{}

c_scriptFuncBase::~c_scriptFuncBase() {}

//file input
void c_scriptFuncBase::read( std::ifstream& fin, scriptMap_t& scrMap ) {
	ulong funcPtr( 0 );
	c_script::read( fin, scrMap );
	fin >> funcPtr;
	nextFunc = dynamic_cast< const c_scriptFuncBase* >( scrMap[ funcPtr ] );
}

//file output
void c_scriptFuncBase::write( std::ofstream& fout ) const {
	c_script::write( fout );
	fout << " " << nextFunc;
}

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
	returnVal( HGE_NULL )
{}

template <>
c_scriptFunc< void* >::c_scriptFunc( const c_scriptFunc& funcCopy ) :
	c_scriptFuncBase( funcCopy ),
	returnVal( HGE_NULL )
{}
