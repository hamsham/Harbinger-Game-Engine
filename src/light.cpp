/* 
 * File:   light.cpp
 * Author: hammy
 * 
 * Created on January 1, 2013, 4:37 AM
 */

#include "light.h"

///////////////////////////////////////////////////////////////////////////////
//		Ambient Lighting
///////////////////////////////////////////////////////////////////////////////
s_light::s_light() :
	color( 1.f, 1.f, 1.f, 1.f ),
	intensity( 1.f )
{}

s_light::s_light( const s_light& al ) :
	color( al.color ),
	intensity( al.intensity )
{}

///////////////////////////////////////////////////////////////////////////////
//		Diffuse Lighting
///////////////////////////////////////////////////////////////////////////////
s_diffuseLight::s_diffuseLight() :
	s_light(),
	dir( 0.f, 0.f, 0.f )
{}

s_diffuseLight::s_diffuseLight( const s_diffuseLight& dl ) :
	s_light( dl ),
	dir( dl.dir )
{}

///////////////////////////////////////////////////////////////////////////////
//		Specular Lighting
///////////////////////////////////////////////////////////////////////////////
s_specularLight::s_specularLight() :
	s_light(),
	shininess( 1.f )
{}

s_specularLight::s_specularLight( const s_specularLight& sl ) :
	s_light( sl ),
	shininess( sl.shininess )
{}

///////////////////////////////////////////////////////////////////////////////
//		Point Lighting
///////////////////////////////////////////////////////////////////////////////
s_pointLight::s_pointLight() :
	s_light(),
	pos( 0.f, 0.f, 0.f ),
	constant( 1.f ),
	linear( 1.f ),
	exponential( 1.f )
{}

s_pointLight::s_pointLight( const s_pointLight& pl ) :
	s_light( pl ),
	pos( pl.pos ),
	constant( pl.constant ),
	linear( pl.linear ),
	exponential( pl.exponential )
{}

///////////////////////////////////////////////////////////////////////////////
//		Spot Lighting
///////////////////////////////////////////////////////////////////////////////
const float s_spotLight::MIN_CONE_ANGLE = HL_PI / 3.15f;

s_spotLight::s_spotLight() :
	s_light(),
	pos( 0.f, 0.f, 0.f ),
	dir( 0.f, 0.f, 0.f ),
	coneAngle( HL_PI_OVR2 ),
	attenuation( 1.f )
{}

s_spotLight::s_spotLight( const s_spotLight& sl ) :
	s_light( sl ),
	pos( sl.pos ),
	dir( sl.dir ),
	coneAngle( sl.coneAngle ),
	attenuation( sl.attenuation )
{}
