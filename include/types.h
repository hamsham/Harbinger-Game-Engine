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
 * Forward Declarations
 */
// Namespaced structures
namespace display {
    struct  videoMode;
}

//simple vertex structure
struct plainVertex {
	vec3 pos;
	vec2 uv;
	vec3 norm;
};

// bumped vertex
struct bumpVertex : plainVertex {
	vec3 tangent;
};

// File-based resources
class   resource;
class   bitmap;
class   cubemap;
class   font;
class   shader;

// Visual-Related
class   billboard;
class   camera;
class   drawTransform;
class   mesh;
class   skybox;
class   string3d;

// Drawable Primitives
class   line;
class   quad;
class   sphere;
class   triangle;

// Lighting types
struct  ambientLight;
struct  diffuseLight;
struct  specularLight;
struct  pointLight;
struct  spotLight;

// Time Types
class   clock;	// time base class
class   timer;		// count down
class   stopwatch;	// count upward, with lapping

} // end hge namespace
#endif	/* __HGE_TYPES_H__ */
