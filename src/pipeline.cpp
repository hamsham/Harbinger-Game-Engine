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

#include "camera.h"
#include "transformations.h"

using namespace hge;

/******************************************************************************
 * Pipeline Construction and Destruction
******************************************************************************/
bool pipeline::init() {
    glGenBuffers( 1, &ubo );
    printGlError( "Creating pipeline uniform block" );
    
    if ( ubo == 0 )
        return false;
    
    updateMatricesImpl();
    return true;
}
void pipeline::terminate() {
    glDeleteBuffers( 1, &ubo );
    ubo = currShader = 0;
    matrixIndexId = GL_INVALID_INDEX;
}

/******************************************************************************
 * Matrix UBO Modification
******************************************************************************/
void pipeline::updateMatricesImpl() {
    // Upload the matrix data to the current shader
    glUniformBlockBinding( currShader, matrixIndexId, HGE_PIPELINE_MATRIX_BINDING );
    glBindBuffer( GL_UNIFORM_BUFFER, ubo );
    glBindBufferBase( GL_UNIFORM_BUFFER, HGE_PIPELINE_MATRIX_BINDING, ubo );

    glBufferData(
        GL_UNIFORM_BUFFER, sizeof( transforms ), transforms, GL_DYNAMIC_DRAW
    );
}

/******************************************************************************
 * Pushing/Popping the matrix stack
******************************************************************************/
void pipeline::applyMatrix( matrixType s, const mat4& m ) {
    // Update the current MVP matrix
    transforms[ s ] = m;
    transforms[ 2 ] = transforms[ HGE_MODEL_MAT ] * transforms[ HGE_VP_MAT ];
    
    updateMatricesImpl();
}

void pipeline::applyMatrix( const drawTransform& obj, const camera& cam ) {
    // Update the current MVP matrix
    transforms[ HGE_MODEL_MAT ] = obj.getModelMatrix();
    transforms[ HGE_VP_MAT ] = cam.getVPMatrix();
    transforms[ 2 ] = transforms[ HGE_MODEL_MAT ] * transforms[ HGE_VP_MAT ];
    
    updateMatricesImpl();
}

void pipeline::removeMatrix( matrixType s ) {
    transforms[ s ] = mat4( 1.f );
}

/******************************************************************************
 * Shader Management
******************************************************************************/
void pipeline::applyStockShader( GLint shaderId ) {
    currShader = shaderId;
    
    // Early bail out
    if ( !currShader ) {
        glUseProgram( 0 );
        return;
    }
    glUseProgram( currShader );
    printGlError( "Applying a shader to the pipeline" );
    
    matrixIndexId = glGetUniformBlockIndex( currShader, "matrixBlock" );
    
    // don't attach the matrix UBO if a binding point doesn't exist
    printGlError( "Accessing the Matrix Uniform Block" );
    
    if ( matrixIndexId == GL_INVALID_INDEX ) {
        return;
    }
    
    glBindBuffer( GL_UNIFORM_BUFFER, ubo );
    glBindBufferBase( GL_UNIFORM_BUFFER, HGE_PIPELINE_MATRIX_BINDING, ubo );
    //glBufferData( GL_UNIFORM_BUFFER, sizeof( transforms ), transforms, GL_DYNAMIC_DRAW );
    
    glUniformBlockBinding( currShader, matrixIndexId, HGE_PIPELINE_MATRIX_BINDING );
    printGlError( "Sending Matrix Uniform Binding" );
    glBindBuffer( GL_UNIFORM_BUFFER, 0 );
}

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

