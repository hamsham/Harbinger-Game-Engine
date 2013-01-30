/* 
 * File:   pipeline_gl_3_3.cpp
 * Author: hammy
 * 
 * Created on December 21, 2012, 1:49 PM
 */

#include <iostream>
#include "pipelineGL.h"

//-----------------------------------------------------------------------------
//	Error Messages
//-----------------------------------------------------------------------------
void printOpenGLError( cstr msg, uint lineNum, cstr sourceFile ) {
		GLenum errorCode( glGetError() );
		if ( errorCode != GL_NO_ERROR ) {
			std::cerr << '\n'
				<< "An OpenGL error has occurred:\n"
				<< "\tInfo: " << msg << '\n'
				<< "\tFile: " << sourceFile << '\n'
				<< "\tLine: " << lineNum << '\n'
				<< "\tCode: 0x" << errorCode << '\n'
				<< "\tStat: " << (int)glGetError() << '\n'
				<< "\tMsg:  ";
	
			switch( errorCode ) {
				case GL_INVALID_ENUM:
					std::cerr << "GL_INVALID_ENUM";
					break;
				case GL_INVALID_VALUE:
					std::cerr << "GL_INVALID_VALUE";
					break;
				case GL_INVALID_OPERATION:
					std::cerr << "GL_INVALID_OPERATION";
					break;
				case GL_INVALID_FRAMEBUFFER_OPERATION:
					std::cerr << "GL_INVALID_FRAMEBUFFER_OPERATION";
					break;
				case GL_OUT_OF_MEMORY:
					std::cerr << "GL_OUT_OF_MEMORY";
					break;
			}
			std::cerr << std::endl;
		}
}

//-----------------------------------------------------------------------------
//	Pipeline Namespace
//-----------------------------------------------------------------------------
GLuint n_pipelineGL::genBufferData(
	GLuint numBuffers, GLuint* pBuffer,
	GLenum target, GLsizeiptr size,
	const void* data, GLenum usage
) {
	glGenBuffers( numBuffers, pBuffer );
	glBindBuffer( target, *pBuffer );
	glBufferData( target, size, data, usage );
	return *pBuffer;
}

