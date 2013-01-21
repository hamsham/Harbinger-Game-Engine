/* 
 * File:   display.cpp
 * Author: hammy
 * 
 * Created on December 16, 2012, 5:12 PM
 */

#include <iostream>
#include <string>
#include "pipelineGL.h"
#include "display.h"

//-----------------------------------------------------------------------------
//	Display Object - Private Variables & functions
//-----------------------------------------------------------------------------
namespace {
	
	//Variables
	int			displayWidth( 1280 );
	int			displayHeight( 720 );
	std::string	windowTitle;
	
	//Functions
	void resize(int width, int height) {
		glViewport( 0, 0, width, height );
		glfwGetWindowSize( &displayWidth, &displayHeight );
	}
	
}// end anonymous namespace
	
//-----------------------------------------------------------------------------
//	Display Object - Initialization
//-----------------------------------------------------------------------------
bool n_display::initScreen() {
	/*/
	 * initialize GLFW
	/*/
	if (!glfwInit()) {
		std::cerr << "GLFW fucked up. Aborting the program." << std::endl;
		return false;
	}
	std::cout << "Number of Processors: " << glfwGetNumberOfProcessors() << std::endl;
	
	//send new display parameters to glfw
	glfwOpenWindowHint	(GLFW_FSAA_SAMPLES, 4);
	glfwOpenWindowHint	(GLFW_OPENGL_VERSION_MAJOR, 3);
	glfwOpenWindowHint	(GLFW_OPENGL_VERSION_MINOR, 3);
	glfwOpenWindowHint	(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwOpenWindowHint	(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwOpenWindowHint	(GLFW_WINDOW_NO_RESIZE, GL_FALSE);
	//create a window
	if (!glfwOpenWindow(displayWidth, displayHeight, 0, 0, 0, 0, 32, 0, GLFW_WINDOW)) {
		std::cerr << "Could not create an OpenGL window using GLFW. Aborting" << std::endl;
		glfwTerminate();
		return false;
	}
	glfwSwapInterval( 0 );
	glfwSetWindowSizeCallback( &resize );
	glfwSetWindowTitle( windowTitle.c_str() );
	glfwDisable( GLFW_STICKY_KEYS );
	glfwDisable( GLFW_MOUSE_CURSOR );
	printGLError( "GLFW Error" );
	
	/*/
	 * initialize GLEW
	/*/
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cerr << "GLEW fucked up. Aborting the program." << std::endl;
		return false;
	}
	std::cout
		<< "Initialized GLEW with status code "
		<< glGetError()
		<< std::endl;
	
	/*/
	 * Initialize OpenGL
	/*/
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glEnable( GL_CULL_FACE );		// Occlusion Culling
	glCullFace( GL_BACK );
	glFrontFace( GL_CCW );
	glEnable( GL_DEPTH_TEST );		// Depth/Z-Buffer
	glDepthFunc( GL_LESS );
	return true;
}

//-----------------------------------------------------------------------------
//	Display Object - Window Title
//-----------------------------------------------------------------------------
void n_display::setWindowTitle( cstr title ) {
	windowTitle = title;
	glfwSetWindowTitle( windowTitle.c_str() );
}

cstr n_display::getWindowTitle() {
	return windowTitle.c_str();
}

//-----------------------------------------------------------------------------
//	Display Object - Screen Size Manipulation
//-----------------------------------------------------------------------------
void n_display::setScreenSize( int x, int y ) {
	glfwSetWindowSize( x, y );
	resize( x, y );
}
	
int n_display::getScreenWidth() {
	return displayWidth;
}

int n_display::getScreenHeight() {
	return displayHeight;
}
