/* 
 * File:   script_variables.cpp
 * Author: hammy
 * 
 * Created on September 25, 2012, 5:21 PM
 */

#include "scripting/script.h"
#include "scripting/script_variables.h"

//-----------------------------------------------------------------------------
//		Integer Variable
//-----------------------------------------------------------------------------
c_scriptInt::c_scriptInt() :
	c_scriptVar( 0 )
{}

c_scriptInt::c_scriptInt( int integer ) :
	c_scriptVar( integer )
{}

c_scriptInt::c_scriptInt( const c_scriptNum& inVar ) :
	c_scriptVar( (int)inVar )
{}

c_scriptInt::~c_scriptInt(){}
		
c_scriptNum c_scriptInt::operator + ( const c_scriptNum& inVar ) const {
	return c_scriptInt( variable + (int)inVar );
}

c_scriptNum c_scriptInt::operator - ( const c_scriptNum& inVar ) const {
	return c_scriptInt( variable - (int)inVar );
}

c_scriptNum c_scriptInt::operator * ( const c_scriptNum& inVar ) const {
	return c_scriptInt( variable * (int)inVar );
}

c_scriptNum c_scriptInt::operator / ( const c_scriptNum& inVar ) const {
	if ( (int)inVar != 0 ) //bwahaha
		return c_scriptInt( variable / (int)inVar );
	return c_scriptInt( 0 );
}

c_scriptNum c_scriptInt::operator % ( const c_scriptNum& inVar ) const {
	return c_scriptInt( variable % (int)inVar );
}

c_scriptNum& c_scriptInt::operator= ( const c_scriptNum& inVar ) {
	variable = (int)inVar;
	return *this;
}

c_scriptNum& c_scriptInt::operator += ( const c_scriptNum& inVar ) {
	variable += (int)inVar;
	return *this;
}

c_scriptNum& c_scriptInt::operator -= ( const c_scriptNum& inVar ) {
	variable -= (int)inVar;
	return *this;
}

c_scriptNum& c_scriptInt::operator *= ( const c_scriptNum& inVar ) {
	variable *= (int)inVar;
	return *this;
}

c_scriptNum& c_scriptInt::operator /= ( const c_scriptNum& inVar ) {
	if ( (int)inVar != 0 )
	{	variable /= (int)inVar;	}
	else
	{	variable = 0;	}
	return *this;
}

c_scriptNum& c_scriptInt::operator %= ( const c_scriptNum& inVar ) {
	variable %= (int)inVar;
	return *this;
}
	
bool c_scriptInt::operator == ( const c_scriptNum& inVar ) const {
	return variable == (int)inVar;
}

bool c_scriptInt::operator != ( const c_scriptNum& inVar ) const {
	return variable != (int)inVar;
}

bool c_scriptInt::operator > ( const c_scriptNum& inVar ) const {
	return variable > (int)inVar;
}

bool c_scriptInt::operator < ( const c_scriptNum& inVar ) const {
	return variable < (int)inVar;
}

bool c_scriptInt::operator >= ( const c_scriptNum& inVar ) const {
	return variable >= (int)inVar;
}

bool c_scriptInt::operator <= ( const c_scriptNum& inVar ) const {
	return variable <= (int)inVar;
}

c_scriptNum& c_scriptInt::operator= ( int inVar ) {
	variable = inVar;
	return *this;
}

c_scriptNum& c_scriptInt::operator= ( unsigned int inVar ) {
	variable = static_cast<int>(inVar);
	return *this;
}

c_scriptNum& c_scriptInt::operator= ( float inVar ) {
	variable = static_cast<int>(inVar);
	return *this;
}

c_scriptInt::operator int() const {
	return variable;
}

c_scriptInt::operator unsigned int() const {
	return static_cast<unsigned int>(variable);
}

c_scriptInt::operator float() const {
	return static_cast<float>(variable);
}

//sibling class
c_scriptInt::operator c_scriptFloat() const {
	return c_scriptFloat( static_cast<float>(variable) );
}

//sibling class
c_scriptInt::operator c_scriptUint() const {
	return c_scriptUint( static_cast<unsigned int>(variable) );
}


//-----------------------------------------------------------------------------
//		Floating Point Variable
//-----------------------------------------------------------------------------
c_scriptFloat::c_scriptFloat() :
	c_scriptVar( 0.0f )
{}

c_scriptFloat::c_scriptFloat( float fpVal ) :
	c_scriptVar( fpVal )
{}

c_scriptFloat::c_scriptFloat( const c_scriptNum& inVar ) :
	c_scriptVar( (float)inVar )
{}

c_scriptFloat::~c_scriptFloat(){}

c_scriptNum c_scriptFloat::operator + ( const c_scriptNum& inVar ) const {
	return c_scriptFloat( variable + (float)inVar );
}

c_scriptNum c_scriptFloat::operator - ( const c_scriptNum& inVar ) const {
	return c_scriptFloat( variable - (float)inVar );
}

c_scriptNum c_scriptFloat::operator * ( const c_scriptNum& inVar ) const {
	return c_scriptFloat( variable * (float)inVar );
}

c_scriptNum c_scriptFloat::operator / ( const c_scriptNum& inVar ) const {
	if ( (float)inVar != 0 ) //bwahaha
		return c_scriptFloat( variable / (float)inVar );
	return c_scriptFloat( 0 );
}

c_scriptNum c_scriptFloat::operator % ( const c_scriptNum& inVar ) const {
	return c_scriptFloat( fmod(variable, (float)inVar) );
}

c_scriptNum& c_scriptFloat::operator= ( const c_scriptNum& inVar ) {
	variable = (float)inVar;
	return *this;
}

c_scriptNum& c_scriptFloat::operator += ( const c_scriptNum& inVar ) {
	variable += (float)inVar;
	return *this;
}

c_scriptNum& c_scriptFloat::operator -= ( const c_scriptNum& inVar ) {
	variable -= (float)inVar;
	return *this;
}

c_scriptNum& c_scriptFloat::operator *= ( const c_scriptNum& inVar ) {
	variable *= (float)inVar;
	return *this;
}

c_scriptNum& c_scriptFloat::operator /= ( const c_scriptNum& inVar ) {
	if ( (float)inVar != 0 )
	{	variable /= (float)inVar;	}
	else
	{	variable = 0;	}
	return *this;
}

c_scriptNum& c_scriptFloat::operator %= ( const c_scriptNum& inVar ) {
	variable = fmod(variable, (float)inVar);
	return *this;
}
		
bool c_scriptFloat::operator == ( const c_scriptNum& inVar ) const {
	return variable == (float)inVar;
}

bool c_scriptFloat::operator != ( const c_scriptNum& inVar ) const {
	return variable != (float)inVar;
}

bool c_scriptFloat::operator > ( const c_scriptNum& inVar ) const {
	return variable > (float)inVar;
}

bool c_scriptFloat::operator < ( const c_scriptNum& inVar ) const {
	return variable < (float)inVar;
}

bool c_scriptFloat::operator >= ( const c_scriptNum& inVar ) const {
	return variable >= (float)inVar;
}

bool c_scriptFloat::operator <= ( const c_scriptNum& inVar ) const {
	return variable <= (float)inVar;
}

c_scriptNum& c_scriptFloat::operator= ( int inVar ) {
	variable = static_cast<float>(inVar);
	return *this;
}

c_scriptNum& c_scriptFloat::operator= ( unsigned int inVar ) {
	variable = static_cast<float>(inVar);
	return *this;
}

c_scriptNum& c_scriptFloat::operator= ( float inVar ) {
	variable = inVar;
	return *this;
}

c_scriptFloat::operator int() const {
	return static_cast<int>(variable);
}

c_scriptFloat::operator unsigned int() const {
	return static_cast<unsigned int>(variable);
}

c_scriptFloat::operator float() const {
	return variable;
}

//sibling class
c_scriptFloat::operator c_scriptInt() const {
	return c_scriptInt( static_cast<int>(variable) );
}

//sibling class
c_scriptFloat::operator c_scriptUint() const {
	return c_scriptUint( static_cast<unsigned int>(variable) );
}


//-----------------------------------------------------------------------------
//		Unsigned Integer Variable
//-----------------------------------------------------------------------------
c_scriptUint::c_scriptUint() :
	c_scriptVar( 0 )
{}

c_scriptUint::c_scriptUint( unsigned int integer ) :
	c_scriptVar( integer )
{}

c_scriptUint::c_scriptUint( const c_scriptNum& inVar ) :
	c_scriptVar( (unsigned int)inVar )
{}

c_scriptUint::~c_scriptUint(){}

c_scriptNum c_scriptUint::operator + ( const c_scriptNum& inVar ) const {
	return c_scriptUint( variable + (unsigned int)inVar );
}

c_scriptNum c_scriptUint::operator - ( const c_scriptNum& inVar ) const {
	return c_scriptUint( variable - (unsigned int)inVar );
}

c_scriptNum c_scriptUint::operator * ( const c_scriptNum& inVar ) const {
	return c_scriptUint( variable * (unsigned int)inVar );
}

c_scriptNum c_scriptUint::operator / ( const c_scriptNum& inVar ) const {
	if ( (unsigned int)inVar != 0 ) //bwahaha
		return c_scriptUint( variable / (unsigned int)inVar );
	return c_scriptUint( 0 );
}

c_scriptNum c_scriptUint::operator % ( const c_scriptNum& inVar ) const {
	return c_scriptUint( variable % (unsigned int)inVar );
}

c_scriptNum& c_scriptUint::operator= ( const c_scriptNum& inVar ) {
	variable = (unsigned int)inVar;
	return *this;
}

c_scriptNum& c_scriptUint::operator += ( const c_scriptNum& inVar ) {
	variable += (unsigned int)inVar;
	return *this;
}

c_scriptNum& c_scriptUint::operator -= ( const c_scriptNum& inVar ) {
	variable -= (unsigned int)inVar;
	return *this;
}

c_scriptNum& c_scriptUint::operator *= ( const c_scriptNum& inVar ) {
	variable *= (unsigned int)inVar;
	return *this;
}

c_scriptNum& c_scriptUint::operator /= ( const c_scriptNum& inVar ) {
	if ( (unsigned int)inVar != 0 )
	{	variable /= (unsigned int)inVar;	}
	else
	{	variable = 0;	}
	return *this;
}

c_scriptNum& c_scriptUint::operator %= ( const c_scriptNum& inVar ) {
	variable %= (unsigned int)inVar;
	return *this;
}

bool c_scriptUint::operator == ( const c_scriptNum& inVar ) const {
	return variable == (unsigned int)inVar;
}

bool c_scriptUint::operator != ( const c_scriptNum& inVar ) const {
	return variable != (unsigned int)inVar;
}

bool c_scriptUint::operator > ( const c_scriptNum& inVar ) const {
	return variable > (unsigned int)inVar;
}

bool c_scriptUint::operator < ( const c_scriptNum& inVar ) const {
	return variable < (unsigned int)inVar;
}

bool c_scriptUint::operator >= ( const c_scriptNum& inVar ) const {
	return variable >= (unsigned int)inVar;
}

bool c_scriptUint::operator <= ( const c_scriptNum& inVar ) const {
	return variable <= (unsigned int)inVar;
}

c_scriptNum& c_scriptUint::operator= ( int inVar ) {
	variable = static_cast<unsigned int>(inVar);
	return *this;
}

c_scriptNum& c_scriptUint::operator= ( unsigned int inVar ) {
	variable = inVar;
	return *this;
}

c_scriptNum& c_scriptUint::operator= ( float inVar ) {
	variable = static_cast<unsigned int>(inVar);
	return *this;
}

c_scriptUint::operator int() const {
	return static_cast<int>(variable);
}

c_scriptUint::operator unsigned int() const {
	return variable;
}

c_scriptUint::operator float() const {
	return static_cast<float>(variable);
}

//sibling class
c_scriptUint::operator c_scriptInt() const {
	return c_scriptInt( static_cast<int>(variable) );
}

//sibling class
c_scriptUint::operator c_scriptFloat() const {
	return c_scriptFloat( static_cast<float>(variable) );
}

//-----------------------------------------------------------------------------
//		Boolean Variable
//-----------------------------------------------------------------------------
c_scriptBool::c_scriptBool() :
	c_scriptVar( false )
{}

c_scriptBool::c_scriptBool( bool inBool ) :
	c_scriptVar( inBool )
{}

c_scriptBool::c_scriptBool( const c_scriptBool& inBool ) :
	c_scriptVar( inBool.variable )
{}

c_scriptBool::~c_scriptBool() {}

void c_scriptBool::setTrue() {
	variable = true;
}

void c_scriptBool::setFalse() {
	variable = false;
}

//-----------------------------------------------------------------------------
//		Character String Variable
//-----------------------------------------------------------------------------
c_scriptString::c_scriptString() {}

c_scriptString::c_scriptString( const char* strCopy ) {
	variable = strCopy;
}

c_scriptString::c_scriptString( const std::string& strCopy ) :
	c_scriptVar( strCopy )
{}

c_scriptString::c_scriptString( const c_scriptString& strCopy ) :
	c_scriptVar( strCopy )
{}

c_scriptString::~c_scriptString() {}

char c_scriptString::operator []( hgeSize_t index ) const {
	HGE_ASSERT( index < variable.size() );
	return variable[ index ];
}

char& c_scriptString::operator []( hgeSize_t index ) {
	HGE_ASSERT( index < variable.size() );
	return variable[ index ];
}

//file input
void c_scriptString::read( std::ifstream& fin, scriptMap_t& scrMap ) {
	c_script::read( fin, scrMap );
	
	std::string::size_type strSize( 0 );
	std::string::size_type iter( 0 );
	fin >> strSize;
	if ( strSize == 0 )
		return;
	fin.get(); // discard the next whitespace before reading in the string
	variable.resize( strSize );
	while ( iter < strSize ) {
		variable[ iter ] = fin.get();
		++iter;
	}
}

//file output
void c_scriptString::write( std::ofstream& fout ) const {
	c_script::write( fout );
	
	fout << " " << variable.size();
	if ( variable.size() != 0 )
		fout << " " << variable.data();
}

//-----------------------------------------------------------------------------
//		3D Vector Variable
//-----------------------------------------------------------------------------

c_scriptVec3::c_scriptVec3() {}

c_scriptVec3::c_scriptVec3( const c_scriptVec3& vecCopy ) :
	c_scriptVar( vecCopy.variable )
{}

c_scriptVec3::c_scriptVec3( const math::vec3& vecCopy ) :
	c_scriptVar( vecCopy )
{}

c_scriptVec3::~c_scriptVec3() {}

float c_scriptVec3::operator[] ( int i ) const {
	return variable[ i ];
}

float& c_scriptVec3::operator[] ( int i ) {
	return variable[ i ];
}
