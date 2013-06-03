/* 
 * File:   main.h
 * Author: hammy
 *
 * Created on August 13, 2012, 9:13 PM
 */

#ifndef __HGE_TYPES_H__
#define	__HGE_TYPES_H__

#include "setup.h"
#include "math/math.h"

namespace hge {

using namespace hamLibs::math;

/*
 * Global Typedefs
 */
typedef unsigned int	uint;
typedef const char*		cstr;

/*
 * Forward Declarations
 */
// Miscellaneous Structures
struct  s_videoMode;
struct  s_vertex;

// Abstract Base Classes
class   c_object;
class   c_drawableObj;

// File-based resources
class   c_resource;
class   c_bitmap;
class   c_cubeMap;
class   c_font;
class   c_shader;

// Visual-Related
class   c_billboard;
class   c_camera;
class   c_mesh;
class   c_skyBox;
class   c_string;

// Drawable Primitives
class   c_line;
class   c_quad;
class   c_sphere;
class   c_triangle;

// Lighting types
struct  s_light;
struct  s_diffuseLight;
struct  s_specularLight;
struct  s_pointLight;
struct  s_spotLight;

// Time Types
class   c_timeObject;	// time base class
class   c_clock;		// general time (counts upwards)
class   c_timer;		// count down
class   c_stopwatch;	// count upward, with lapping

} // end hge namespace
#endif	/* __HGE_TYPES_H__ */
