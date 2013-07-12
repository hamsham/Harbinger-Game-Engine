/* 
 * File:   pipeline_gl_3_3.cpp
 * Author: hammy
 * 
 * Created on December 21, 2012, 1:49 PM
 */

#include <iostream>
#include <cstddef> /* used for offsetof */

#include "math/math.h"
#include "pipeline.h"

using namespace hge;

/******************************************************************************
 * Enabling Vertex Attributes
******************************************************************************/
void pipeline::enablePlainVertexAttribs() {
	glEnableVertexAttribArray( VERTEX_ATTRIB );
	glEnableVertexAttribArray( TEXTURE_ATTRIB );
	glEnableVertexAttribArray( NORMAL_ATTRIB );
    
	glVertexAttribPointer(
		VERTEX_ATTRIB, ARRAY_COUNT_FROM_SIZE( plainVertex::pos.v ), GL_FLOAT,
        GL_FALSE, sizeof( plainVertex ), (GLvoid*)offsetof( plainVertex, pos.v )
	);
    
	glVertexAttribPointer(
		TEXTURE_ATTRIB, ARRAY_COUNT_FROM_SIZE( plainVertex::uv.v ), GL_FLOAT,
        GL_FALSE, sizeof( plainVertex ), (GLvoid*)offsetof( plainVertex, uv.v )
	);
    
	glVertexAttribPointer(
		NORMAL_ATTRIB, ARRAY_COUNT_FROM_SIZE( plainVertex::norm.v ), GL_FLOAT,
        GL_FALSE, sizeof( plainVertex ), (GLvoid*)offsetof( plainVertex, norm.v )
	);
    
	glDisableVertexAttribArray( TANGENT_ATTRIB );
	glDisableVertexAttribArray( BITANGENT_ATTRIB );
    
	printGlError( "Error while enabling plain vertex attributes.");
}

void pipeline::enableBumpVertexAttribs() {
	glEnableVertexAttribArray( VERTEX_ATTRIB );
	glEnableVertexAttribArray( TEXTURE_ATTRIB );
	glEnableVertexAttribArray( NORMAL_ATTRIB );
    glEnableVertexAttribArray( TANGENT_ATTRIB );
    glEnableVertexAttribArray( BITANGENT_ATTRIB );
    
	glVertexAttribPointer(
		VERTEX_ATTRIB, ARRAY_COUNT_FROM_SIZE( bumpVertex::pos.v ), GL_FLOAT,
        GL_FALSE, sizeof( bumpVertex ), (GLvoid*)offsetof( bumpVertex, pos.v )
	);
    
	glVertexAttribPointer(
		TEXTURE_ATTRIB, ARRAY_COUNT_FROM_SIZE( bumpVertex::uv.v ), GL_FLOAT,
        GL_FALSE, sizeof( bumpVertex ), (GLvoid*)offsetof( bumpVertex, uv.v )
	);
    
	glVertexAttribPointer(
		NORMAL_ATTRIB, ARRAY_COUNT_FROM_SIZE( bumpVertex::norm.v ), GL_FLOAT,
        GL_FALSE, sizeof( bumpVertex ), (GLvoid*)offsetof( bumpVertex, norm.v )
	);
    
	glVertexAttribPointer(
		TANGENT_ATTRIB, ARRAY_COUNT_FROM_SIZE( bumpVertex::tng.v ), GL_FLOAT,
        GL_FALSE, sizeof( bumpVertex ), (GLvoid*)offsetof( bumpVertex, tng.v )
	);
    
	glVertexAttribPointer(
		BITANGENT_ATTRIB, ARRAY_COUNT_FROM_SIZE( bumpVertex::btng.v ), GL_FLOAT,
        GL_FALSE, sizeof( bumpVertex ), (GLvoid*)offsetof( bumpVertex, btng.v )
	);
    
	printGlError( "Error while enabling bumped vertex attributes.");
}

/******************************************************************************
 * Error Messages
******************************************************************************/
void pipeline::printErrorMsg( const char* msg, unsigned lineNum, const char* sourceFile ) {
	GLenum errorCode( glGetError() );
	if ( errorCode == GL_NO_ERROR )
		return;
	
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
		default:
			std::cerr << "Unknown Error";
	}
	std::cerr << std::endl;
}

