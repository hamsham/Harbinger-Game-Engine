/* 
 * File:   main.h
 * Author: hammy
 *
 * Created on August 13, 2012, 9:13 PM
 */

/*-----------------------------------------------------------------------------
 * This is the Second OpenGL Graphics Test using GLEW
 * This test makes more extensive use of transformations and camera controls
-----------------------------------------------------------------------------*/

#ifndef __HGE_TYPES_H__
#define	__HGE_TYPES_H__

#include "defs/preprocessor.h"

/*
 * Macros
 */
#ifndef HGE_NULL
	#define HGE_NULL nullptr
#endif

#ifndef ARRAY_SIZE_FROM_ELEMENTS
	#define ARRAY_SIZE_FROM_ELEMENTS( a ) ( sizeof( a ) / sizeof( a[ 0 ] ) )
#endif

#ifdef DEBUG
	#ifndef HGE_ASSERT
		#include <cassert>
		#define HGE_ASSERT( x ) assert( x )
	#endif
#else
	#ifndef HGE_ASSERT
		#define HGE_ASSERT( x )
	#endif
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

/*
 * Global Typedefs
 */
typedef unsigned int	uint;
typedef unsigned long	ulong;
typedef unsigned char	uchar;
typedef const char*		cstr;
typedef unsigned char	byte;
typedef unsigned long	hgeSize_t;

/*
 * Global Variables
 */
const float	PROG_UPDATE_SPEED	= (1.0f / 60);
const int	MAX_NUM_LIGHTS		= 10;
const float	PI					= 3.1415926535897932f;
const float	TWO_PI				= 6.2831853071795864f;
const float	PI_OVER_2			= (PI / 2);
const float	PI_OVER_4			= (PI / 4);
const float	EPSILON				= 1e-5;
cstr const	RESOURCE_DIR		= "resources/";
cstr const	WRONG_TEXTURE_FILE	= "resources/wrong.jpg";

/*
 * Forward Declarations
 */
class	c_object;
class	c_drawableObj;
class	c_resource;
class	c_bitmap;
class	c_image;
struct	c_shader;
class	c_camera;
struct	c_vertex;
class	c_mesh;
class	s_light;
class	s_diffuseLight;
class	s_specularLight;
class	s_pointLight;
class	s_spotLight;
class	c_timeObject;	// time base class
class	c_clock;		// general time (counts upwards)
class	c_timer;		// count down
class	c_stopwatch;	// count upward, with lapping

#endif	/* __HGE_TYPES_H__ */
