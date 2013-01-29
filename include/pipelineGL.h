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

//Error Messaging
HGE_API void printOpenGLError( cstr msg, uint lineNum, cstr sourceFile );
#ifdef DEBUG
	#define printGLError( x ) printOpenGLError( x, __LINE__, __FILE__ )
#else
	#define printGLError( x )
#endif

//OpenGL pipeline convenience functions
namespace n_pipelineGL {
	
	HGE_API GLuint genBufferData(
		GLuint numBuffers, GLuint* pBuffer,
		GLenum target, GLsizeiptr size,
		const void* data, GLenum usage
	);
	
}

#endif	/* __HGE_PIPELINE_GL_H__ */

