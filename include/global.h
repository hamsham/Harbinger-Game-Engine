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

#ifndef __HGE_GLOBAL_H__
#define	__HGE_GLOBAL_H__

#include <GL/glew.h>
#include <GL/glfw.h>

/*
 * Forward Declarations
 */
class	c_object;
class	c_drawableObj;
class	c_resource;
class	c_bitmap;
struct	c_shader;
class	c_camera;
struct	material;
struct	c_vertex;
class	c_mesh;
class	c_light;
class	c_ambientLight;
class	c_diffuseLight;
class	c_specularLight;
struct	compundLight;

/*
 * Macros
 */
#define HGE_NULL nullptr
#define ARRAY_SIZE_FROM_ELEMENTS( a ) ( sizeof( a ) / sizeof( a[ 0 ] ) )

/*
 * Global Typedefs
 */
typedef unsigned int	uint;
typedef unsigned char	uchar;
typedef const char*		cstr;
typedef unsigned char	byte;

/*
 * Global Variables
 */

const float	PROG_UPDATE_SPEED = 1.0f / 60;

const float	PI			= 3.1415926535897932f;
const float	TWO_PI		= 6.2831853071795864f;
const float	PI_OVER_2	= PI / 2;
const float	PI_OVER_4	= PI / 4;
const float	EPSILON		= 1e-5;

cstr const	RESOURCE_DIR		= "resources/";

#endif	/* __HGE_GLOBAL_H__ */
