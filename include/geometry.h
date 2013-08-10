/* 
 * File:   geometry.h
 * Author: hammy
 *
 * Created on June 8, 2013, 10:41 PM
 */

#ifndef __HGE_GEOMETRY_H__
#define	__HGE_GEOMETRY_H__

#include "types.h"

namespace hge {

//simple vertex structures
struct plainVertex {
	vec3 pos;
	vec2 uv;
	vec3 norm;
};

// bumped vertex
struct bumpVertex {
	vec3 pos;
	vec2 uv;
	vec3 norm;
	vec3 tng;
    vec3 btng;
};

HGE_API vec3 calcNormal( const vec3& v0, const vec3& v1, const vec3& v2 );
HGE_API void calcTangents( bumpVertex& v0, bumpVertex& v1, bumpVertex& v2 );

} // end hge namespace

#endif	/* __HGE_GEOMETRY_H__ */
