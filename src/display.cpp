/* 
 * File:   display.cpp
 * Author: hammy
 * 
 * Created on December 16, 2012, 5:12 PM
 */

#include <iostream>
#include <vector>
#include "pipeline.h"
#include <GLFW/glfw3.h>
#include "display.h"

namespace hge {

//-----------------------------------------------------------------------------
//	Display Object - Static Methods and Members
//-----------------------------------------------------------------------------
bool window::displayInitialized = false;
vec2i window::deskResolution = vec2i( 0, 0 );

const vec2i& window::getDesktopSize() {
    const GLFWvidmode* pVidMode = glfwGetVideoMode( glfwGetPrimaryMonitor() );
    deskResolution[0] = pVidMode->width;
    deskResolution[1] = pVidMode->height;
	return deskResolution;
}

//-----------------------------------------------------------------------------
//	Move Operators
//-----------------------------------------------------------------------------
window::window( window&& w ) :
    displayFullscreen( w.displayFullscreen ),
    pContext( w.pContext ),
    resolution( w.resolution )
{
    w.displayFullscreen = false;
    w.pContext = nullptr;
    w.resolution = vec2i( DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT );
}

window& window::operator = ( window&& w ) {
    displayFullscreen   = w.displayFullscreen;
    w.displayFullscreen = false;
    
    pContext            = w.pContext;
    w.pContext          = nullptr;
    
    resolution          = w.resolution;
    w.resolution        = vec2i( DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT );
}

//-----------------------------------------------------------------------------
//	Window Creation
//-----------------------------------------------------------------------------
window::window(
    int w, int h,
    bool resizeable,
    bool fullscreen,
    bool useVsync
) {
    GLFWmonitor* fsMonitor = fullscreen ? glfwGetPrimaryMonitor() : nullptr;
    
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
    
    pContext = glfwCreateWindow( w, h, "Harbinger Game Engine", fsMonitor, nullptr );
    HGE_ASSERT( pContext != nullptr );
    
    glfwMakeContextCurrent( pContext );
    
    glfwGetWindowSize( pContext, &resolution[0], &resolution[1] );
    glfwSetInputMode( pContext, GLFW_STICKY_KEYS, GL_TRUE );
    
    glfwSwapInterval( useVsync );
    
    displayFullscreen = fullscreen;
    
    HGE_ASSERT( pipeline::init( resolution ) );
}

//-----------------------------------------------------------------------------
//	Window Termination
//-----------------------------------------------------------------------------
window::~window() {
    pipeline::terminate();
    glfwDestroyWindow( pContext );
    pContext = nullptr;
}

//-----------------------------------------------------------------------------
//	Window Open-ness check
//-----------------------------------------------------------------------------
bool window::isOpen() {
    return pContext != nullptr;
}

//-----------------------------------------------------------------------------
//	Display Initialization & Termination
//-----------------------------------------------------------------------------
bool window::init() {
    if ( displayInitialized )
        return true;
    
    std::cout << "Initializing display subsystems...";
    displayInitialized = glfwInit();
	if ( displayInitialized == false ) {
		std::cerr << "Display failed to initialize." << std::endl;
		return false;
    }
    
    const GLFWvidmode* deskMode = glfwGetVideoMode( glfwGetPrimaryMonitor() );
    deskResolution[0] = deskMode->width;
    deskResolution[1] = deskMode->height;
    
    std::cout << "Done." << std::endl;
    
    return displayInitialized;
}

void window::terminate() {
    pipeline::terminate();
    glfwTerminate();
    displayInitialized = false;
    deskResolution = vec2i( 0 );
}

void window::flip() {
    glfwSwapBuffers( pContext );
    glfwPollEvents();
}

//-----------------------------------------------------------------------------
//	Display Object - Screen Size Manipulation
//-----------------------------------------------------------------------------
void window::resize( const vec2i& newSize ) {
    // Let GLFW handle resolutions. Acquire the new size when it's finished.
    glfwSetWindowSize( pContext, newSize[0], newSize[1] );
    glfwGetWindowSize( pContext, &resolution[0], &resolution[1] );
}

const vec2i& window::getResolution() {
    glfwGetWindowSize( pContext, &resolution[0], &resolution[1] );
	return resolution;
}

//-----------------------------------------------------------------------------
//	Window View Manipulation
//-----------------------------------------------------------------------------
void window::raise() {
    glfwRestoreWindow( pContext );
    
}

void window::minimize() {
    glfwIconifyWindow( pContext );
    
}

//-----------------------------------------------------------------------------
//	Window View Manipulation
//-----------------------------------------------------------------------------
void window::setTitle( const char* str ) {
    glfwSetWindowTitle(  pContext , str );
}

} // end hge namespace
