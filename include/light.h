/* 
 * File:   light.h
 * Author: hammy
 *
 * Created on January 1, 2013, 4:37 AM
 */

#ifndef __HGE_LIGHT_H__
#define	__HGE_LIGHT_H__

#include <GL/glew.h>
#include "hamLibs.h"
#include "types.h"
using namespace hamLibs;

///////////////////////////////////////////////////////////////////////////////
//		Lighting Abstraction
///////////////////////////////////////////////////////////////////////////////
class c_light {
	private:
		math::vec4	color;
		float		intensity;
		
	public:
		c_light			();
		c_light			( const c_light& );
		virtual ~c_light	() = 0;
		
		void				setIntensity	( float amount )				{ intensity = amount; }
		float				getIntensity	() const						{ return intensity; }
		
		void				setColor		( const math::vec4& inCol )		{ color = inCol; }
		const math::vec4&	getColor		() const						{ return color; }
};

///////////////////////////////////////////////////////////////////////////////
//		Ambient Lighting
///////////////////////////////////////////////////////////////////////////////
class c_ambientLight : virtual public c_light {
	public:
		c_ambientLight		();
		c_ambientLight		( const c_ambientLight& );
		~c_ambientLight		() {}
};

///////////////////////////////////////////////////////////////////////////////
//		Diffuse Lighting
///////////////////////////////////////////////////////////////////////////////
class c_diffuseLight : virtual public c_light {
	private:
		math::vec3	dir;
		
	public:
		c_diffuseLight		();
		c_diffuseLight		( const c_diffuseLight& );
		~c_diffuseLight		() {};
		
		void				setDir				( const math::vec3& inDir )		{ dir = inDir; }
		const math::vec3&	getDir				() const						{ return dir; }
};

///////////////////////////////////////////////////////////////////////////////
//		Specular Lighting
///////////////////////////////////////////////////////////////////////////////
class c_specularLight : virtual public c_light {
	private:
		GLfloat		shininess;
		
	public:
		c_specularLight();
		c_specularLight( const c_specularLight& );
		~c_specularLight() {}
		
		void				setShininess		( float amt )					{ shininess = amt; }
		float				getShininess		() const						{ return shininess; }
};

#endif	/* __HGE_LIGHT_H__ */

