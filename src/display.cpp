/* 
 * File:   display.cpp
 * Author: hammy
 * 
 * Created on December 16, 2012, 5:12 PM
 */

#include <iostream>
#include <vector>
#include "pipeline.h"
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
    std::vector< hge::s_videoMode > vidModes;
    
}// end anonymous namespace

namespace hge {
	
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
    
    /*/
     * Create a new window using GLFW
    /*/
	glfwOpenWindowHint( GLFW_OPENGL_VERSION_MAJOR, 3 );
	glfwOpenWindowHint( GLFW_OPENGL_VERSION_MINOR, 3 );
	glfwOpenWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
	glfwOpenWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
	glfwOpenWindowHint( GLFW_WINDOW_NO_RESIZE, (resizeable ? GL_FALSE : GL_TRUE) );
    
	if ( !glfwOpenWindow(
            w, h, 8, 8, 8, 8, 24, 24,
            fullscreen ? GLFW_FULLSCREEN : GLFW_WINDOW
    ) ) {
		std::cerr << "Failed to create an OpenGL context." << std::endl;
        return false;
	}
    
    glfwGetWindowSize( &windowWidth, &windowHeight );
    glfwSetWindowPos( 0, 0 );
    
	if ( useVsync == false )
        glfwSwapInterval( 0 );
    
	/*/
	 * Initialize GLEW
	/*/
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
	
	/*/
	 * Default OpenGL parameters
	/*/
    glViewport( 0, 0, windowWidth, windowHeight );
	glClearColor( 0.5f, 0.5f, 0.5f, 1.0f );
	glEnable    ( GL_CULL_FACE );		// Occlusion Culling
	glCullFace  ( GL_BACK );
	glFrontFace ( GL_CCW );
	glEnable    ( GL_DEPTH_TEST );		// Depth/Z-Buffer
	glDepthFunc ( GL_LESS );
    
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
    
    glfwCloseWindow();
    displayFullscreen = false;
}

//-----------------------------------------------------------------------------
//	Window Open-ness check
//-----------------------------------------------------------------------------
bool display::isWindowOpen() {
    return glfwGetWindowParam(GLFW_OPENED) == GL_TRUE;
}

//-----------------------------------------------------------------------------
//	Display Initialization & Termination
//-----------------------------------------------------------------------------
bool display::init() {
    if ( displayInitialized )
        return true;
    
    displayInitialized = glfwInit();
	if ( displayInitialized == false ) {
		std::cerr << "Display failed to initialize." << std::endl;
		return false;
    }
    
    GLFWvidmode deskMode;
    glfwGetDesktopMode( &deskMode );
    deskWidth = deskMode.Width;
    deskHeight = deskMode.Height;
    
    std::cout << "Acquiring display modes...";
    GLFWvidmode modeList[ MAX_VIDEO_MODES ];
    int numModes = glfwGetVideoModes( modeList, MAX_VIDEO_MODES );
    vidModes.resize( numModes );
    
    for ( int i = 0; i < numModes; ++i ) {
        s_videoMode* pMode = &vidModes[ i ];
        
        pMode->bpp
            = modeList[ i ].BlueBits
            + modeList[ i ].GreenBits
            + modeList[ i ].RedBits;
        
        pMode->width = modeList[i].Width;
        pMode->height = modeList[i].Height;
    }
    std::cout << "Done. Acquired " << numModes << " video modes." << std::endl;
    
    return displayInitialized;
}

void display::terminate() {
    if ( isWindowOpen() )
        display::closeWindow();
    glfwTerminate();
    vidModes.clear();
    displayInitialized = false;
    deskWidth = deskHeight = 0;
}

void display::flip() {
    glfwSwapBuffers();
}

//-----------------------------------------------------------------------------
//	Display Object - Screen Size Manipulation
//-----------------------------------------------------------------------------
void display::resizeWindow( int w, int h ) {
    // Get the display size that matches an acquired display mode
    for ( int i = 0; i < vidModes.size(); ++i ) {
        if ( w == vidModes[i].width && h == vidModes[i].height ) {
            glfwSetWindowSize( w, h );
            glViewport( 0, 0, w, h );
            break;
        }
    }
    glfwGetWindowSize( &windowWidth, &windowHeight );
}

int display::getWindowWidth() {
    glfwGetWindowSize( &windowWidth, &windowHeight );
	return windowWidth;
}

int display::getWindowHeight() {
    glfwGetWindowSize( &windowWidth, &windowHeight );
	return windowHeight;
}

int display::getDesktopWidth() {
	return deskWidth;
}

int display::getDesktopHeight() {
	return deskHeight;
}

void display::setResizeCallback( void(*callback)( int w, int h ) ) {
    glfwSetWindowSizeCallback( callback );
}

//-----------------------------------------------------------------------------
//	Window View Manipulation
//-----------------------------------------------------------------------------
void display::raiseWindow() {
    glfwRestoreWindow();
    
}

void display::lowerWindow() {
    glfwIconifyWindow();
    
}

//-----------------------------------------------------------------------------
//	Window View Manipulation
//-----------------------------------------------------------------------------
void display::setWindowTitle( const char* str ) {
    glfwSetWindowTitle( str );
}

//-----------------------------------------------------------------------------
//	Desktop Video Modes
//-----------------------------------------------------------------------------
int display::getNumVideoModes() {
    return vidModes.size();
}

const s_videoMode* display::getVideoMode( int i ) {
    return ( (i > 0) && (i <= vidModes.size()) )
        ? &vidModes[i]
        : nullptr;
}

} // end hge namespace
