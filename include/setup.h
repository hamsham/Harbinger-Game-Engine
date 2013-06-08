/* 
 * File:   setup.h
 * Author: hammy
 *
 * Created on February 23, 2013, 11:38 AM
 * 
 * This file includes mutually used compiler macros
 */

#ifndef __HGE_SETUP_H__
#define	__HGE_SETUP_H__

#include "defs/preprocessor.h"
#include "utils/assert.h"

/*
 * Macros
 */
// Returns the number of elements in a statically allocated array.
// This will not work for dynamic arrays (will return 1)
#ifndef ARRAY_COUNT_FROM_SIZE
	#define ARRAY_COUNT_FROM_SIZE( a ) ( sizeof( a ) / sizeof( a[ 0 ] ) )
#endif

#ifdef DEBUG
    #define HGE_ASSERT( x ) ASSERT_WARN( x )
#else
    #define HGE_ASSERT( X ) ASSERT_FATAL( x )
#endif

/*
 * Dynamic Library Setup
 */
#ifdef HL_OS_WINDOWS
    #if defined(HGE_BUILD_DYNAMIC)
        #define HGE_API __declspec( dllexport )
    #else
        #define HGE_API __declspec( dllimport )
    #endif
#else
    #define HGE_API
#endif

/*
 * Function Inlining
 */
#ifndef HGE_INLINE
	#define HGE_INLINE HL_INLINE
#endif

/*
 * Compile-Time hashing
 */
#ifndef HGE_HASH
    #define HGE_HASH( x ) hamLibs::utils::constHash( HL_STRINGIFY( x ) )
#endif

#endif	/* __HGE_SETUP_H__ */

