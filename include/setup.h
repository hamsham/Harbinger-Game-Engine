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
    #define HGE_ASSERT( x ) HL_WARN( x )
#else
    #define HGE_ASSERT( x ) HL_ASSERT( x )
#endif

/*
 * Dynamic Library Setup
 */
#ifdef HL_OS_WINDOWS
    #if defined(HGE_BUILD_DYNAMIC)
        #define HGE_API __declspec( dllexport )
    #elif defined(HGE_BUILD_STATIC)
        #define HGE_API
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
 * Engine Default Values
 */
#ifndef HGE_DEFAULT_WINDOW_WIDTH
    #define HGE_DEFAULT_WINDOW_WIDTH 640
#endif

#ifndef HGE_DEFAULT_WINDOW_HEIGHT
    #define HGE_DEFAULT_WINDOW_HEIGHT 480
#endif

#endif	/* __HGE_SETUP_H__ */

