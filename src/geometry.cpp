/* 
 * File:   geometry.cpp
 * Author: hammy
 * 
 * Created on June 8, 2013, 10:41 PM
 */

#include "geometry.h"

namespace hge {

/* Code for the tangent and biTangent calculation is credited towards:
 * OpenGL-Tutorial.org, Tutorial 13 (Normal Mapping)
 * 
 * http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-13-normal-mapping/
 */
void calcTangents( bumpVertex& v0, bumpVertex& v1, bumpVertex& v2 ) {
    const vec3 deltaPos1    = v1.pos - v0.pos;
    const vec3 deltaPos2    = v2.pos - v0.pos;
    const vec2 deltaUv1     = v1.uv - v0.uv;
    const vec2 deltaUv2     = v2.uv - v0.uv;
    float r = 1.f / ( (deltaUv1[0] * deltaUv2[1]) - (deltaUv1[1] * deltaUv2[0]) );
    
    v0.tng = v1.tng = v2.tng =
        vec3( (deltaPos1 * deltaUv2[1]) - (deltaPos2 * deltaUv1[1]) ) * r;
    
    v0.btng = v1.btng = v2.btng =
        vec3( (deltaPos2 * deltaUv1[0]) - (deltaPos1 * deltaUv2[0]) ) * r;
}

} // end hge namespace
