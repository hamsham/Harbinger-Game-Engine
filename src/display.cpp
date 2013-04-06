/* 
 * File:   display.cpp
 * Author: hammy
 * 
 * Created on December 16, 2012, 5:12 PM
 */

#include <iostream>
#include <string>
#include "pipeline.h"
#include "display.h"

namespace hge {

//-----------------------------------------------------------------------------
//	Display Object - Private Variables & functions
//-----------------------------------------------------------------------------
namespace {
	
	int displayWidth( 0 );
	int displayHeight( 0 );
	
//-----------------------------------------------------------------------------
//	GLFW - Window Context Initialization (private)
//-----------------------------------------------------------------------------
bool initGLFW( int w, int h ) {
	if ( !glfwInit() ) {
		std::cerr << "GLFW failed to initialize." << std::endl;
		return false;
	}
	
	glfwOpenWindowHint	( GLFW_OPENGL_VERSION_MAJOR, 3);
	glfwOpenWindowHint	( GLFW_OPENGL_VERSION_MINOR, 3 );
	glfwOpenWindowHint	( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
	glfwOpenWindowHint	( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
	glfwOpenWindowHint	( GLFW_WINDOW_NO_RESIZE, GL_FALSE );
    
	//create a window
	if ( !glfwOpenWindow( w, h, 8, 8, 8, 8, 16, 16, GLFW_WINDOW ) ) {
		std::cerr << "Failed to create an OpenGL context using GLFW." << std::endl;
		glfwTerminate();
        return false;
	}
    
	glfwSwapInterval( 0 ); // disables VSync
    
    // using a lambda to set the window resize callback
	glfwSetWindowSizeCallback(
        []( int width, int height )->void {
            glViewport( 0, 0, width, height );
            displayWidth = width;
            displayHeight = height;
        }
    );
    
	printGLError( "GLFW Error" );
    
    return true;
}

//-----------------------------------------------------------------------------
//	GLEW - OpenGL 3.3 Initialization (private)
//-----------------------------------------------------------------------------
bool initGLEW() {
	glewExperimental = GL_TRUE;
	if ( glewInit() != GLEW_OK ) {
		std::cerr << "GLEW failed to initialize" << std::endl;
		return false;
	}
	std::cout << "Initialized GLEW with status code " << glGetError() << std::endl;
	
	/*/
	 * Initialize OpenGL
	/*/
	glClearColor( 0.5f, 0.5f, 0.5f, 1.0f );
	glEnable    ( GL_CULL_FACE );		// Occlusion Culling
	glCullFace  ( GL_BACK );
	glFrontFace ( GL_CCW );
	glEnable    ( GL_DEPTH_TEST );		// Depth/Z-Buffer
	glDepthFunc ( GL_LESS );
    return true;
}

}// end anonymous namespace

//-----------------------------------------------------------------------------
//	Display Object - Initialization & Termination
//-----------------------------------------------------------------------------
bool n_display::init( int w, int h ) {
	if ( !initGLFW( w, h ) || !initGLEW() ) {
        return false;
    }
    return true;
}

void n_display::terminate() {
    glfwTerminate();
}

//-----------------------------------------------------------------------------
//	Display Object - Screen Size Manipulation
//-----------------------------------------------------------------------------
void n_display::resizeWindow( int w, int h ) {
    glfwSetWindowSize( w, h );
}

int n_display::getScreenWidth() {
	return displayWidth;
}

int n_display::getScreenHeight() {
	return displayHeight;
}

} // end hge namespace
