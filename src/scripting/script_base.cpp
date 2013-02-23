/* 
 * File:   script_base.cpp
 * Author: hammy
 * 
 * Created on September 25, 2012, 5:18 PM
 */

#include "scripting/script.h"
#include "scripting/script_base.h"

//-----------------------------------------------------------------------------
//		Script Base Class
//-----------------------------------------------------------------------------
c_script::~c_script() {}

void c_script::read( std::istream& stin, scriptMap_t& scriptMap ) {
	void* ptr( nullptr );
	stin >> ptr >> xPos >> yPos;
	scriptMap[ ptr ] = this;
}

void c_script::write( std::ostream& stout ) const {
	stout << this << ' ' << xPos << ' ' << yPos;
}

//-----------------------------------------------------------------------------
//		Variable Base Class
//-----------------------------------------------------------------------------
c_varBase::~c_varBase() {}

//-----------------------------------------------------------------------------
//		Variable Interface Classe
//		Abstract
//		All datas MUST support copy construction & assignment operators
//-----------------------------------------------------------------------------
template <>
void c_varBool::read( std::istream& fin, scriptMap_t& scrMap ) {
	c_var::read( fin, scrMap );
	int inVar( 0 );
	fin >> inVar;
	data = (inVar == 0); // a file I/O bug occurred due to uninitialized data
}

template <>
void c_varBool::write( std::ostream& fout ) const {
	c_var::write( fout );
	fout << " " << (data != 0); // a file I/O bug occurred due to uninitialized data
}

template <>
void c_varVec3::read( std::istream& fin, scriptMap_t& scrMap ) {
	c_var::read( fin, scrMap );
	fin >> data[0] >> data[1] >> data[2];
}

template <>
void c_varVec3::write( std::ostream& fout ) const {
	c_var::write( fout );
	fout << " " << data[0] << " " << data[1] << " " << data[2];
}

template <>
void c_varString::read( std::istream& fin, scriptMap_t& scrMap ) {
	c_var::read( fin, scrMap );
	
	std::string::size_type strSize( 0 );
	std::string::size_type iter( 0 );
	fin >> strSize;
	if ( strSize == 0 )
		return;
	fin.get(); // discard the next whitespace before reading in the string
	data.resize( strSize );
	while ( iter < strSize ) {
		data[ iter ] = fin.get();
		++iter;
	}
}

template <>
void c_varString::write( std::ostream& fout ) const {
	c_var::write( fout );
	
	fout << " " << data.size();
	if ( data.size() != 0 )
		fout << " " << data.data();
}

//-----------------------------------------------------------------------------
//		Function Base Class
//-----------------------------------------------------------------------------
c_funcBase::~c_funcBase() {}

void c_funcBase::read( std::istream& stin, scriptMap_t& scriptMap ) {
	void* nextPtr( nullptr );
	void* retPtr( nullptr );
	
	c_script::read( stin, scriptMap );
	stin >> nextPtr >> retPtr;
	
	nextFunc = dynamic_cast< c_funcBase* >( scriptMap[ nextPtr ] ); 
	retVal = dynamic_cast< c_varBase* >( scriptMap[ retPtr ] ); 
}

void c_funcBase::write( std::ostream& stout ) const {
	c_script::write( stout );
	stout
		<< ' ' << static_cast< c_script* >( nextFunc )
		<< ' ' << static_cast< c_script* >( retVal );
}
