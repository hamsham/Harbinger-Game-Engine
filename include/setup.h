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

/*
 * Macros
 */
#ifndef HGE_NULL
	#define HGE_NULL nullptr
#endif

// Returns the number of elements in a statically allocated array.
// This will not work for dynamic arrays (will return 1)
#ifndef ARRAY_SIZE_FROM_ELEMENTS
	#define ARRAY_SIZE_FROM_ELEMENTS( a ) ( sizeof( a ) / sizeof( a[ 0 ] ) )
#endif

// HL_ASSERT is only defined when 'DEBUG' has also been defined
#ifndef HGE_ASSERT
    #define HGE_ASSERT( x ) HL_ASSERT( x )
#endif

/*
 * Dynamic Library Setup
 */
#if (defined (HL_OS_WINDOWS) && defined (HGE_BUILD_DYNAMIC))
	#define HGE_API __declspec( dllexport )
#elif (defined (HL_OS_WINDOWS) && defined (HGE_API_DYNAMIC))
	#define HGE_API __declspec( dllimport )
#else
	#define HGE_API
#endif

/*
 * Function Inlining
 */
#ifndef HGE_INLINE
	#define HGE_INLINE HL_INLINE
#endif

#endif	/* __HGE_SETUP_H__ */

