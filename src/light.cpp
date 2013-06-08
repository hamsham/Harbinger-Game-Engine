/* 
 * File:   light.cpp
 * Author: hammy
 * 
 * Created on January 1, 2013, 4:37 AM
 */

#include "light.h"

namespace hge {

///////////////////////////////////////////////////////////////////////////////
//		Ambient Lighting
///////////////////////////////////////////////////////////////////////////////
ambientLight::ambientLight() :
	color( 1.f, 1.f, 1.f, 1.f ),
	intensity( 1.f )
{}

ambientLight::ambientLight( const ambientLight& al ) :
	color( al.color ),
	intensity( al.intensity )
{}

///////////////////////////////////////////////////////////////////////////////
//		Diffuse Lighting
///////////////////////////////////////////////////////////////////////////////
diffuseLight::diffuseLight() :
	ambientLight(),
	dir( 0.f, 0.f, 0.f )
{}

diffuseLight::diffuseLight( const diffuseLight& dl ) :
	ambientLight( dl ),
	dir( dl.dir )
{}

///////////////////////////////////////////////////////////////////////////////
//		Specular Lighting
///////////////////////////////////////////////////////////////////////////////
specularLight::specularLight() :
	ambientLight(),
	shininess( 1.f )
{}

specularLight::specularLight( const specularLight& sl ) :
	ambientLight( sl ),
	shininess( sl.shininess )
{}

///////////////////////////////////////////////////////////////////////////////
//		Point Lighting
///////////////////////////////////////////////////////////////////////////////
pointLight::pointLight() :
	ambientLight(),
	pos( 0.f, 0.f, 0.f ),
	constant( 1.f ),
	linear( 1.f ),
	exponential( 1.f )
{}

pointLight::pointLight( const pointLight& pl ) :
	ambientLight( pl ),
	pos( pl.pos ),
	constant( pl.constant ),
	linear( pl.linear ),
	exponential( pl.exponential )
{}

///////////////////////////////////////////////////////////////////////////////
//		Spot Lighting
///////////////////////////////////////////////////////////////////////////////
const float spotLight::MIN_CONE_ANGLE = HL_PI / 3.15f;

spotLight::spotLight() :
	ambientLight(),
	pos( 0.f, 0.f, 0.f ),
	dir( 0.f, 0.f, 0.f ),
	coneAngle( HL_PI_OVR2 ),
	attenuation( 1.f )
{}

spotLight::spotLight( const spotLight& sl ) :
	ambientLight( sl ),
	pos( sl.pos ),
	dir( sl.dir ),
	coneAngle( sl.coneAngle ),
	attenuation( sl.attenuation )
{}

} // end hge namespace
