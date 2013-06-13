/* 
 * File:   display.cpp
 * Author: hammy
 * 
 * Created on December 16, 2012, 5:12 PM
 */

#include <iostream>
#include <vector>
#include "pipeline.h"
#include <GL/glfw3.h>
#include "display.h"

//-----------------------------------------------------------------------------
//	Display Object - Private Variables & functions
//-----------------------------------------------------------------------------
namespace {
    
    bool    displayInitialized  = false;
    bool    displayFullscreen   = false;
    int     windowWidth         = hge::display::DEFAULT_WINDOW_WIDTH;
    int     windowHeight        = hge::display::DEFAULT_WINDOW_HEIGHT;
    int     deskWidth           = 0;
    int     deskHeight          = 0;
    
    const int MAX_VIDEO_MODES   = 100;
    
}// end anonymous namespace
GLFWwindow* mainWindow          = nullptr; // needed by the input system

namespace hge {

display::hgeWindow* display::getCurrentWindow() {
    return mainWindow;
}

//-----------------------------------------------------------------------------
//	Window Creation
//-----------------------------------------------------------------------------
bool display::createWindow(
    int w, int h,
    bool resizeable,
    bool fullscreen,
    bool useVsync
) {
    if ( isWindowOpen() == true )
        return true;
    
    /*
     * Create a new window using GLFW
     */
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
	glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
	glfwWindowHint( GLFW_RESIZABLE, (resizeable ? GL_TRUE : GL_FALSE) );
#ifdef DEBUG
    glfwWindowHint( GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE );
#endif
    
    mainWindow = glfwCreateWindow( w, h, "Harbinger Game Engine", nullptr, nullptr );
	if ( !mainWindow ) {
		std::cerr << "Failed to create an OpenGL context." << std::endl;
        return false;
	}
    glfwMakeContextCurrent( mainWindow );
    
    glfwGetWindowSize( mainWindow, &windowWidth, &windowHeight );
    glfwSetInputMode( mainWindow, GLFW_STICKY_KEYS, GL_TRUE );
    
    glfwSwapInterval( useVsync );
    
	/*
	 * Initialize GLEW
	 */
	glewExperimental = GL_TRUE;
	if ( glewInit() != GLEW_OK ) {
		std::cerr << "Post-window creation error" << std::endl;
        closeWindow();
		return false;
	}
	std::cout
        << "Created a window. OpenGL 3.3 initialized.\n\t0x"
        << std::hex << glGetError()
        << std::dec << std::endl;
    
    displayFullscreen = fullscreen;
    
    // Initialize all successive operations which required the window to be open
    return pipeline::init();
}

//-----------------------------------------------------------------------------
//	Window Termination
//-----------------------------------------------------------------------------
void display::closeWindow() {
    // Close all OpenGL-dependant functions
    pipeline::terminate();
    
    glfwDestroyWindow( mainWindow );
    
    mainWindow = false;
    displayFullscreen = false;
}

//-----------------------------------------------------------------------------
//	Window Open-ness check
//-----------------------------------------------------------------------------
bool display::isWindowOpen() {
    return mainWindow != nullptr;
}

//-----------------------------------------------------------------------------
//	Display Initialization & Termination
//-----------------------------------------------------------------------------
bool display::init() {
    if ( displayInitialized )
        return true;
    
    std::cout << "Initializing display subsystems...";
    displayInitialized = glfwInit();
	if ( displayInitialized == false ) {
		std::cerr << "Display failed to initialize." << std::endl;
		return false;
    }
    
    const GLFWvidmode* deskMode = glfwGetVideoMode( glfwGetPrimaryMonitor() );
    deskWidth = deskMode->width;
    deskHeight = deskMode->height;
    
    std::cout << "Done." << std::endl;
    
    return displayInitialized;
}

void display::terminate() {
    if ( isWindowOpen() ) {
        pipeline::terminate();
        display::closeWindow();
    }
    glfwTerminate();
    displayInitialized = false;
    deskWidth = deskHeight = 0;
}

void display::flip() {
    glfwSwapBuffers( mainWindow );
    glfwPollEvents();
}

//-----------------------------------------------------------------------------
//	Display Object - Screen Size Manipulation
//-----------------------------------------------------------------------------
void display::resizeWindow( int w, int h ) {
    glfwGetWindowSize( mainWindow, &windowWidth, &windowHeight );
}

int display::getWindowWidth() {
    glfwGetWindowSize( mainWindow, &windowWidth, &windowHeight );
	return windowWidth;
}

int display::getWindowHeight() {
    glfwGetWindowSize( mainWindow, &windowWidth, &windowHeight );
	return windowHeight;
}

int display::getDesktopWidth() {
	return deskWidth;
}

int display::getDesktopHeight() {
	return deskHeight;
}

//-----------------------------------------------------------------------------
//	Window View Manipulation
//-----------------------------------------------------------------------------
void display::raiseWindow() {
    glfwRestoreWindow( mainWindow );
    
}

void display::lowerWindow() {
    glfwIconifyWindow( mainWindow );
    
}

//-----------------------------------------------------------------------------
//	Window View Manipulation
//-----------------------------------------------------------------------------
void display::setWindowTitle( const char* str ) {
    glfwSetWindowTitle(  mainWindow , str );
}

} // end hge namespace
