/* 
 * File:   script_variables.cpp
 * Author: hammy
 * 
 * Created on September 25, 2012, 5:21 PM
 */

#include "scripting/script_variables.h"

//-----------------------------------------------------------------------------
//		Variable Specializations
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
	fin >> strSize;
	if ( strSize == 0 ) return;
    
	fin.get(); // discard the next whitespace before reading in the string
	data.resize( strSize );
        
    for ( std::string::size_type iter( 0 ); iter < strSize; ++iter )
        data[ iter ] = fin.get();
}

template <>
void c_varString::write( std::ostream& fout ) const {
	c_var::write( fout );
	
	fout << " " << data.size();
	if ( data.size() != 0 )
		fout << " " << data.data();
}
