/* 
 * File:   hash.h
 * Author: hammy
 *
 * Created on February 23, 2013, 12:01 PM
 */

#ifndef __HL_HASH_H__
#define	__HL_HASH_H__

#include <string>

namespace hamLibs {
namespace utils {

//-----------------------------------------------------------------------------
// compile-time string hashing using DJB2
// This method was found on here:
// http://nguillemot.blogspot.com/2012/06/side-story-compile-time-string-hashing.html
//-----------------------------------------------------------------------------
// this function is reserved for "constHash()" and should not be used
constexpr unsigned int constHashRecursive( const char* str, unsigned int hashVal ) {
	return ( !*str )
	? hashVal
	: constHashRecursive( str+1, ((hashVal << 5) + hashVal) ^ *str );
}

//-----------------------------------------------------------------------------
// this function expects a NULL-Terminated string
//-----------------------------------------------------------------------------
constexpr unsigned int constHash( const char* str ) {
	return ( !str ) ? 0 : constHashRecursive( str, 5381 );
}

//-----------------------------------------------------------------------------
// Basic Hashing using the C++ Standard Library <locale>
//-----------------------------------------------------------------------------
long int runtimeHash( const std::string& s );

} // end utils namespace
} // end hamlibs namespace

#endif	/* __HL_HASH_H__ */

