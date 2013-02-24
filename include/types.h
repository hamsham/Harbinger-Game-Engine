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

#include "setup.h"
#include "math/math.h"
using namespace hamLibs::math;

/*
 * Global Typedefs
 */
typedef unsigned int	uint;
typedef unsigned long	ulong;
typedef unsigned char	uchar;
typedef const char*		cstr;
typedef unsigned char	byte;
typedef unsigned long	hgeSize_t;
typedef vec3_t< int >	vec3i;
typedef vec2_t< int >	vec2i;

/*
 * Global Variables
 */
constexpr float	PROG_UPDATE_SPEED	= (1.0f / 60);
constexpr int	MAX_NUM_LIGHTS		= 10;
constexpr float	PI					= 3.1415926535897932f;
constexpr float	TWO_PI				= 6.2831853071795864f;
constexpr float	PI_OVER_2			= (PI / 2);
constexpr float	PI_OVER_4			= (PI / 4);
constexpr float	EPSILON				= 1e-5;
constexpr char*	RESOURCE_DIR		= "resources/";
constexpr char*	WRONG_TEXTURE_FILE	= "resources/wrong.jpg";

/*
 * Forward Declarations
 */
class	c_object;
class	c_drawableObj;
class	c_resource;
class	c_bitmap;
class	c_image;
class	c_shader;
class	c_camera;
struct	s_vertex;
class	c_mesh;
struct	s_light;
struct	s_diffuseLight;
struct	s_specularLight;
struct	s_pointLight;
struct	s_spotLight;
class	c_timeObject;	// time base class
class	c_clock;		// general time (counts upwards)
class	c_timer;		// count down
class	c_stopwatch;	// count upward, with lapping

#endif	/* __HGE_TYPES_H__ */
