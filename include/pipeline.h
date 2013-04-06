/* 
 * File:   pipeline_gl_3_3.h
 * Author: hammy
 *
 * Created on December 21, 2012, 1:49 PM
 */

#ifndef __HGE_PIPELINE_GL_H__
#define	__HGE_PIPELINE_GL_H__

#include <GL/glew.h>
#include <GL/glfw.h>
#include "types.h"

namespace hge {

//Error Messaging
HGE_API void printOpenGLError( cstr msg, uint lineNum, cstr sourceFile );

//OpenGL pipeline convenience functions
namespace n_pipeline {
	
	HGE_API GLuint genBufferData(
		GLuint numBuffers, GLuint* pBuffer,
		GLenum target, GLsizeiptr size,
		const void* data, GLenum usage
	);
	
	//void setMVP( const mat4& modelMatrix, const mat4& viewProjectionMatrix );
	
}

} // end hge namespace

#ifdef DEBUG
	#define printGLError( x ) hge::printOpenGLError( x, __LINE__, __FILE__ )
#else
	#define printGLError( x )
#endif

#endif	/* __HGE_PIPELINE_GL_H__ */
