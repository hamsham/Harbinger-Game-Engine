/* 
 * File:   pipeline_gl_3_3.cpp
 * Author: hammy
 * 
 * Created on December 21, 2012, 1:49 PM
 */

#include <iostream>
#include "math/math.h"
#include "pipeline.h"
#include "camera.h"
#include "display.h"
#include "shader.h"
#include "stockShaders.h"
#include "primitives.h"
#include "transformations.h"

using namespace hge;

/******************************************************************************
 * Private matrices for initial stack population
******************************************************************************/
namespace {
    GLuint ubo = 0;
    GLuint matrixIndexId = GL_INVALID_INDEX;
    GLuint currShader = 0;
    
    // Creating an array of four 4x4 Matrices. Model, View, Projection, and MVP
    mat4 transforms[ 3 ] = { mat4( 1.f ) };
}

/******************************************************************************
 * Pushing to the matrix stack
******************************************************************************/
void pipeline::applyMatrix( e_matrixState s, const mat4& m ) {
    
    // Update the current MVP matrix
    transforms[ s ] = m;
    transforms[ 2 ] = transforms[ HGE_MODEL_MAT ] * transforms[ HGE_VP_MAT ];
    
    // Upload the matrix data to the current shader
    glUniformBlockBinding( currShader, matrixIndexId, HGE_PIPELINE_MATRIX_BINDING );
    glBindBuffer( GL_UNIFORM_BUFFER, ubo );
    glBindBufferBase( GL_UNIFORM_BUFFER, HGE_PIPELINE_MATRIX_BINDING, ubo );
    
    glBufferData(
        GL_UNIFORM_BUFFER, sizeof( transforms ), transforms, GL_DYNAMIC_DRAW
    );
}

void pipeline::applyMatrix( const drawTransform& obj, const camera& cam ) {
    
    // Update the current MVP matrix
    transforms[ HGE_MODEL_MAT ] = obj.getModelMatrix();
    transforms[ HGE_VP_MAT ] = cam.getVPMatrix();
    transforms[ 2 ] = transforms[ HGE_MODEL_MAT ] * transforms[ HGE_VP_MAT ];
    
    // Upload the matrix data to the current shader
    glUniformBlockBinding( currShader, matrixIndexId, HGE_PIPELINE_MATRIX_BINDING );
    glBindBuffer( GL_UNIFORM_BUFFER, ubo );
    glBindBufferBase( GL_UNIFORM_BUFFER, HGE_PIPELINE_MATRIX_BINDING, ubo );
    
    glBufferData(
        GL_UNIFORM_BUFFER, sizeof( transforms ), transforms, GL_DYNAMIC_DRAW
    );
}

/******************************************************************************
 * Popping off a matrix from the stack
******************************************************************************/
void pipeline::removeMatrix( e_matrixState s ) {
    transforms[ s ] = mat4( 1.f );
}

/******************************************************************************
 * Shader Management
******************************************************************************/
void pipeline::applyShader( GLuint programId ) {
    currShader = programId;
    
    if ( !currShader )
        return;
    
    glUseProgram( currShader );
    printGlError( "Applying a shader to the pipeline" );
    
    glBindBuffer( GL_UNIFORM_BUFFER, ubo );
    glBindBufferBase( GL_UNIFORM_BUFFER, HGE_PIPELINE_MATRIX_BINDING, ubo );
    glBufferData( GL_UNIFORM_BUFFER, sizeof( transforms ), transforms, GL_DYNAMIC_DRAW );
    
    matrixIndexId = glGetUniformBlockIndex( currShader, "matrixBlock" );
    printGlError( "Accessing the Matrix Uniform Block" );
    
#ifdef DEBUG
    if ( matrixIndexId == GL_INVALID_INDEX ) {
        std::cerr
            << "ERROR: Incompatible shader detected. "
            << "Please ensure that the current shader has a uniform block "
            << "titled \"matrixBlock\" which contains the mat4 identifiers "
            << "\"modelMatrix\", \"viewMatrix\", "
            << "\"projMatrix\", and \"mvpMatrix\"."
            << std::endl;
        
        return;
    }
#endif
    
    glUniformBlockBinding( currShader, matrixIndexId, HGE_PIPELINE_MATRIX_BINDING );
    printGlError( "Sending Matrix Uniform Binding" );
    glBindBuffer( GL_UNIFORM_BUFFER, 0 );
}

void pipeline::applyShader( const shader& s ) {
    applyShader( s.getProgramId() );
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

/******************************************************************************
 * Pipeline Initialization
******************************************************************************/
bool pipeline::init() {
    
    ASSERT_FATAL( display::isWindowOpen() );
	
	/*
	 * Default OpenGL parameters
	 */
    glViewport( 0, 0, display::getWindowWidth(), display::getWindowHeight() );
	glClearColor( 0.5f, 0.5f, 0.5f, 1.0f );
	glEnable    ( GL_CULL_FACE );		// Occlusion Culling
	glCullFace  ( GL_BACK );
	glFrontFace ( GL_CCW );
	glEnable    ( GL_DEPTH_TEST );		// Depth/Z-Buffer
	glDepthFunc ( GL_LESS );
    
    if ( ubo != 0 ) {
        // Return if the pipeline is already initialized
        return true;
    }

    glGenBuffers( 1, &ubo );
    printGlError( "Creating pipeline uniform block" );

    if ( ubo == 0 ) {
        std::cerr
            << "An error occurred while initializing the graphics pipeline."
            << std::endl;
        return false;
    }
    
    // PRIMITIVE INITIALIZATION
    if ( !initPrimitives() || !stockShaders::init() ) {
        terminate();
        return false;
    }
    
    return true;
}

/******************************************************************************
 * Pipeline Termination
******************************************************************************/
void pipeline::terminate() {
    if ( ubo == 0 )
        return;
    
    glDeleteBuffers( 1, &ubo );
    ubo = currShader = 0;
    matrixIndexId = GL_INVALID_INDEX;
    
    terminatePrimitives();
    stockShaders::terminate();
}
