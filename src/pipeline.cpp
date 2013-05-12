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
#include "primitives.h"

using namespace hamLibs;
using namespace hge;

/******************************************************************************
 * Private matrices for initial stack population
******************************************************************************/
namespace {
    GLuint ubo = 0;
    GLuint matrixIndexId = GL_INVALID_INDEX;
    GLuint currShader = 0;
    
    // Creating an array of four 4x4 Matrices. Model, View, Projection, and MVP
    math::mat4 transforms[ 4 ] = {
        math::mat4( 1.f ),
        math::mat4( 1.f ),
        math::perspective(
            c_camera::DEFAULT_FOV,
            c_camera::DEFAULT_ASPECT_WIDTH / c_camera::DEFAULT_ASPECT_HEIGHT,
            c_camera::DEFAULT_Z_NEAR, c_camera::DEFAULT_Z_FAR
        ),
        math::mat4( 1.f ),
    };
    
    const char* matrixNames[] = {
        "matrixBlock.modelMatrix",
        "matrixBlock.viewMatrix",
        "matrixBlock.projMatrix",
        "matrixBlock.mvpMatrix"
    };
    
    GLuint matrixIndices[4] = { 0 };
    GLuint matrixStrides[4] = { 0 };
    GLuint matrixOffsets[4] = { 0 };
}

/******************************************************************************
 * Pushing to the matrix stack
******************************************************************************/
void pipeline::applyMatrix( e_matrixState s, const math::mat4& m ) {
    
    transforms[ s ] = m;
    
    // Update the current MVP matrix
    transforms[ 3 ]
        = transforms[ HGE_MODEL_MAT ]
        * transforms[ HGE_VIEW_MAT ]
        * transforms[ HGE_PROJ_MAT ];
    
    // Check if there's a current shader
    if ( !currShader )
        return;
    
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
    // Default perspective projection matrix
    if ( s == HGE_PROJ_MAT ) {
        transforms[ s ] = math::perspective(
            c_camera::DEFAULT_FOV,
            (float)display::getScreenWidth() / display::getScreenHeight(),
            c_camera::DEFAULT_Z_NEAR, c_camera::DEFAULT_Z_FAR
        );
    }
    // Default model & view matrices
    else {
        transforms[ s ] = math::mat4( 1.f );
    }
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
    
    glUniformBlockBinding( currShader, matrixIndexId, HGE_PIPELINE_MATRIX_BINDING );
    printGlError( "Sending Matrix Uniform Binding" );
    glBindBuffer( GL_UNIFORM_BUFFER, 0 );
}

/******************************************************************************
 * Error Messages
******************************************************************************/
void pipeline::printGlErrorMsg( cstr msg, uint lineNum, cstr sourceFile ) {
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
    if ( !initPrimitives() ) {
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
}
