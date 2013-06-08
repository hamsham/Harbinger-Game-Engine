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
//		Ambient Lighting
///////////////////////////////////////////////////////////////////////////////
struct HGE_API ambientLight {
	vec4    color;
	float   intensity;

	ambientLight    ();
	ambientLight    ( const ambientLight& );
	~ambientLight   () {}
};

///////////////////////////////////////////////////////////////////////////////
//		Diffuse Lighting
///////////////////////////////////////////////////////////////////////////////
struct HGE_API diffuseLight : ambientLight {
    vec3    dir;

    diffuseLight    ();
    diffuseLight    ( const diffuseLight& );
    ~diffuseLight   () {};
};

///////////////////////////////////////////////////////////////////////////////
//		Specular Lighting
///////////////////////////////////////////////////////////////////////////////
struct HGE_API specularLight : ambientLight {
    float   shininess;
    
    specularLight     ();
    specularLight     ( const specularLight& );
    ~specularLight    () {}
};

///////////////////////////////////////////////////////////////////////////////
//		Point Lighting
///////////////////////////////////////////////////////////////////////////////
struct HGE_API pointLight : ambientLight {
    vec3    pos;
    float   constant;
    float   linear;
    float   exponential;

    pointLight    ();
    pointLight    ( const pointLight& );
    ~pointLight   () {}
};

///////////////////////////////////////////////////////////////////////////////
//		Spot Lighting
///////////////////////////////////////////////////////////////////////////////
struct HGE_API spotLight : ambientLight {
    static const float MIN_CONE_ANGLE;
    vec3    pos;
    vec3    dir;
    float   coneAngle;
    float   attenuation;

    spotLight     ();
    spotLight     ( const spotLight& );
    ~spotLight    () {}
};

} // end hge namespace

#endif	/* __HGE_LIGHT_H__ */
