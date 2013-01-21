/* 
 * File:   shader.h
 * Author: hammy
 *
 * Created on August 12, 2012, 10:29 PM
 */

#ifndef __HGE_SHADER_H__
#define __HGE_SHADER_H__

#include <vector>
#include <GL/glew.h>
#include "resource.h"

///////////////////////////////////////////////////////////////////////////////
//		OpenGL Shader Class
///////////////////////////////////////////////////////////////////////////////
class c_shader : virtual public c_resource {
	private:
		std::vector< GLuint > shaderID;
		GLuint progID;
		
		void printError	( GLuint shdrID = 0 ) const;
	
	public:
		static const GLuint		INVALID_UNIFORM;
		static const GLuint		VERTEX_ATTRIB;
		static const GLuint		TEXTURE_ATTRIB;
		static const GLuint		NORMAL_ATTRIB;
		
		c_shader	();
		~c_shader	();
		
		bool	isLoaded		() const { return (progID) ? true : false; }
		bool	load			( cstr shaderFilePath, int shaderType );
		bool	compile			();
		void	unload			();
		GLuint	getProgramID	() const { return progID; }
};

///////////////////////////////////////////////////////////////////////////////
//		Stock (prebuilt) Shaders
///////////////////////////////////////////////////////////////////////////////
namespace n_stockShaders {
	bool	init();
	GLuint	getTexture2dShader();
	GLuint	getAmbientLightShader();
	GLuint	getDiffuseLightShader();
	GLuint	getSpecularLightShader();
}

#endif	/* __HGE_SHADER_H__ */
