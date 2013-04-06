/* 
 * File:   light.h
 * Author: hammy
 *
 * Created on January 1, 2013, 4:37 AM
 */

#ifndef __HGE_LIGHT_H__
#define	__HGE_LIGHT_H__

#include <GL/glew.h>
#include "math/math.h"
#include "types.h"

namespace hge {

///////////////////////////////////////////////////////////////////////////////
//		Ambient Lighting
///////////////////////////////////////////////////////////////////////////////
struct HGE_API s_light {
	vec4	color;
	float	intensity;

	s_light			();
	s_light			( const s_light& );
	~s_light		() {}
};

///////////////////////////////////////////////////////////////////////////////
//		Diffuse Lighting
///////////////////////////////////////////////////////////////////////////////
struct HGE_API s_diffuseLight : s_light {
	vec3	dir;

	s_diffuseLight		();
	s_diffuseLight		( const s_diffuseLight& );
	~s_diffuseLight		() {};
};

///////////////////////////////////////////////////////////////////////////////
//		Specular Lighting
///////////////////////////////////////////////////////////////////////////////
struct HGE_API s_specularLight : s_light {
	GLfloat	shininess;

	s_specularLight		();
	s_specularLight		( const s_specularLight& );
	~s_specularLight	() {}
};

///////////////////////////////////////////////////////////////////////////////
//		Point Lighting
///////////////////////////////////////////////////////////////////////////////
struct HGE_API s_pointLight : s_light {
	vec3	pos;
	float	constant;
	float	linear;
	float	exponential;

	s_pointLight		();
	s_pointLight		( const s_pointLight& );
	~s_pointLight		() {}
};

///////////////////////////////////////////////////////////////////////////////
//		Spot Lighting
///////////////////////////////////////////////////////////////////////////////
struct HGE_API s_spotLight : s_light {
	static const float MIN_CONE_ANGLE;
	vec3	pos;
	vec3	dir;
	float	coneAngle;
	float	attenuation;

	s_spotLight			();
	s_spotLight			( const s_spotLight& );
	~s_spotLight		() {}
};

} // end hge namespace

#endif	/* __HGE_LIGHT_H__ */
