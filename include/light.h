/* 
 * File:   light.h
 * Author: hammy
 *
 * Created on January 1, 2013, 4:37 AM
 */

#ifndef __HGE_LIGHT_H__
#define	__HGE_LIGHT_H__

#include "types.h"

namespace hge {

///////////////////////////////////////////////////////////////////////////////
//		Color Base Structure
///////////////////////////////////////////////////////////////////////////////
struct baseLight {
    vec4    color       = vec4( 1.f );
    float   intensity   = 1.f;
    
    virtual ~baseLight() = 0;
};

inline baseLight::~baseLight() {}

///////////////////////////////////////////////////////////////////////////////
//		Ambient Lighting
///////////////////////////////////////////////////////////////////////////////
struct ambientLight : baseLight {
};

///////////////////////////////////////////////////////////////////////////////
//		Diffuse Lighting
///////////////////////////////////////////////////////////////////////////////
struct diffuseLight : baseLight {
    vec3 dir = vec3( 0.f, 0.f, -1.f );
};

///////////////////////////////////////////////////////////////////////////////
//		Specular Lighting
///////////////////////////////////////////////////////////////////////////////
struct specularLight : baseLight {
    float shininess = 1.f;
};

///////////////////////////////////////////////////////////////////////////////
//		Point Lighting
///////////////////////////////////////////////////////////////////////////////
struct pointLight : baseLight {
    vec3    pos         = vec3( 0.f );
    float   constant    = 1.f;
    float   exponential = 1.f;
    float   linear      = 1.f;
};

///////////////////////////////////////////////////////////////////////////////
//		Spot Lighting
///////////////////////////////////////////////////////////////////////////////
struct spotLight : baseLight {
    vec3    pos         = vec3( 0.f );
    vec3    dir         = vec3( 0.f, 0.f, -1.f );
    float   coneAngle   = HL_PI_INVERSE * 3.15f;
    float   attenuation = 1.f;
};

} // end hge namespace

#endif	/* __HGE_LIGHT_H__ */
