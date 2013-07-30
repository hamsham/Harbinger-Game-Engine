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
#include "primitives.h"

namespace hge {

//-----------------------------------------------------------------------------
//	Display Object - Static Methods and Members
//-----------------------------------------------------------------------------
window::displayList_t window::displayList;
std::vector< window::modeList_t > window::modeList;

window::display* window::getMainMonitor() {
    return glfwGetPrimaryMonitor();
}

void window::setActiveContext( context* c ) {
    glfwMakeContextCurrent( c );
}

vec2i window::getDesktopSize() {
    const GLFWvidmode* pVidMode = glfwGetVideoMode( glfwGetPrimaryMonitor() );
    return vec2i( pVidMode->width, pVidMode->height );
}

void window::monitorCallback( hge::window::display* d, int action ) {
    if ( action == GLFW_CONNECTED ) {
        displayList.push_back( d );
        return;
    }
    
    for ( unsigned i = 0; i < displayList.size(); ++i ) {
        if ( displayList[ i ] == d && action == GLFW_DISCONNECTED ) {
            displayList.erase( displayList.begin() + i );
            break;
        }
    }
}

//-----------------------------------------------------------------------------
//	Display Initialization & Termination
//-----------------------------------------------------------------------------
bool window::init() {
    // initialize GLFW
    std::cout << "Initializing display subsystems...";
	if ( !glfwInit() ) {
		std::cerr << "Display failed to initialize." << std::endl;
		return false;
    }
    
    // populate the list of monitors
    int numDisplays = 0;
    GLFWmonitor** displays = glfwGetMonitors( &numDisplays );
    displayList.resize( numDisplays, nullptr );
    modeList.resize( numDisplays );
    
    for ( int i = 0; i < numDisplays; ++i )
        displayList[ i ] = displays[ i ];
    
    // populate the list of display modes
    for ( int i = 0; i < displayList.size(); ++i ) {
        int numDisplayModes = 0;
        const GLFWvidmode* modeArray = glfwGetVideoModes( displayList[ i ], &numDisplayModes );
        
        modeList[ i ].resize( numDisplayModes );
        
        // Copy GLFW's video mode array into harbinger
        for ( int j = 0; j < numDisplayModes; ++j ) {
            modeList[ i ][ j ].width        = modeArray[ j ].width;
            modeList[ i ][ j ].height       = modeArray[ j ].height;
            modeList[ i ][ j ].redBits      = modeArray[ j ].redBits;
            modeList[ i ][ j ].blueBits     = modeArray[ j ].blueBits;
            modeList[ i ][ j ].greenBits    = modeArray[ j ].greenBits;
            modeList[ i ][ j ].refreshRate  = modeArray[ j ].refreshRate;
        }
    }
    
    std::cout << "Done." << std::endl;
    return displayList.size() > 0;
}

void window::terminate() {
    glfwTerminate();
    
    displayList.clear();
    modeList.clear();
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
    display* fullscreen,
    bool useVsync
) {
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
    
    pContext = glfwCreateWindow( w, h, "Harbinger Game Engine", fullscreen, nullptr );
    HGE_ASSERT( pContext != nullptr );
    
    glfwMakeContextCurrent  ( pContext );
    glfwGetWindowSize       ( pContext, &resolution[0], &resolution[1] );
    glfwSetInputMode        ( pContext, GLFW_STICKY_KEYS, GL_TRUE );
    glfwSwapInterval        ( useVsync );
    
    displayFullscreen = fullscreen != nullptr;
    
	/*
	 * Initialize GLEW
	 */
	glewExperimental = GL_TRUE; // Ensure core extensions are loaded
    if ( glewInit() != GLEW_OK ) {
        glfwDestroyWindow( pContext );
        HGE_ASSERT( false );
    }
    
	std::cout
        << "Created a window. OpenGL 3.3 initialized.\n\t0x"
        << std::hex << glGetError()
        << std::dec << std::endl;
	
	/*
	 * Default OpenGL parameters
	 */
    glViewport  ( 0, 0, resolution[0], resolution[1] );
	glClearColor( 0.f, 0.f, 0.f, 1.0f );
	glEnable    ( GL_CULL_FACE );		// Occlusion Culling
	glCullFace  ( GL_BACK );
	glFrontFace ( GL_CCW );
	glEnable    ( GL_DEPTH_TEST );		// Depth/Z-Buffer
	glDepthFunc ( GL_LEQUAL );
}

//-----------------------------------------------------------------------------
//	Window Termination
//-----------------------------------------------------------------------------
window::~window() {
    glfwDestroyWindow( pContext );
    glfwDefaultWindowHints();
    pContext = nullptr;
}

//-----------------------------------------------------------------------------
//	Window Open-ness check
//-----------------------------------------------------------------------------
bool window::isOpen() {
    return pContext != nullptr;
}

//-----------------------------------------------------------------------------
//	Display Object - Flip the window's backbuffer
//-----------------------------------------------------------------------------
void window::flip() {
    glfwSwapBuffers         ( pContext );
    glfwPollEvents          ();
}

//-----------------------------------------------------------------------------
//	Display Object - Screen Size Manipulation
//-----------------------------------------------------------------------------
const vec2i& window::getResolution() {
    glfwGetWindowSize( pContext, &resolution[0], &resolution[1] );
	return resolution;
}

void window::setResolution( const vec2i& newSize ) {
    // Let GLFW handle resolutions. Acquire the new size when it's finished.
    glfwSetWindowSize( pContext, newSize[0], newSize[1] );
    glfwGetWindowSize( pContext, &resolution[0], &resolution[1] );
}

vec2i window::getPosition() {
    vec2i pos;
    glfwGetWindowPos( pContext, &pos[0], &pos[1] );
    return pos;
}

void window::setPosition( const vec2i& p ) {
    glfwSetWindowPos( pContext, p[0], p[1] );
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

void window::showForeground() {
    glfwShowWindow( pContext );
}

//-----------------------------------------------------------------------------
//	Window View Manipulation
//-----------------------------------------------------------------------------
void window::setTitle( const char* str ) {
    glfwSetWindowTitle(  pContext , str );
}

//-----------------------------------------------------------------------------
//	Window Destruction handling
//-----------------------------------------------------------------------------
void window::notifyClose( bool c ) {
    glfwSetWindowShouldClose( pContext, (int)c );
}

bool window::wantsToClose() const {
    glfwWindowShouldClose( pContext ) == 0;
}

//-----------------------------------------------------------------------------
//	Window Callbacks
//-----------------------------------------------------------------------------
void window::setCloseCallback( void (*func)( context* ) ) {
    glfwSetWindowCloseCallback( pContext, func );
}

void window::setPositionCallback( void (*func)( context*, int, int ) ) {
    glfwSetWindowPosCallback( pContext, func );
}

void window::setResizeCallback( void (*func)( context*, int, int ) ) {
    glfwSetWindowSizeCallback( pContext, func );
}

void window::setRefreshCallback( void (*func)( context* ) ) {
    glfwSetWindowRefreshCallback( pContext, func );
}

void window::setFocusCallback( void (*func)( context*, int ) ) {
    glfwSetWindowFocusCallback( pContext, func );
}

void window::setIconifyCallback( void (*func)( context*, int ) ) {
    glfwSetWindowIconifyCallback( pContext, func );
}

} // end hge namespace
