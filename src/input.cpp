
#include <GL/glfw.h>
#include "input.h"

using namespace hge;

///////////////////////////////////////////////////////////////////////////////
//      INPUT CALLBACKS
///////////////////////////////////////////////////////////////////////////////
void input::setKeyCallback( void(*func)(int,int) ) {
    glfwSetKeyCallback( func );
}

void input::setTextCallback( void(*func)(int,int) ) {
    glfwSetCharCallback( func );
}

void input::setMouseCallback( void(*func)(int,int) ) {
    glfwSetMouseButtonCallback( func );
}

void input::setMousePosCallback( void(*func)(int,int) ) {
    glfwSetMousePosCallback( func );
}

void input::setMouseWheelCallback( void(*func)(int) ) {
    glfwSetMouseWheelCallback( func );
}

///////////////////////////////////////////////////////////////////////////////
//      BUTTON PRESSES
///////////////////////////////////////////////////////////////////////////////
bool input::isKeyPressed( int keycode ) {
    return glfwGetKey( keycode ) == GLFW_PRESS;
}

bool input::isMouseButtonPressed( int button ) {
    return glfwGetMouseButton( button ) == GLFW_PRESS;
}

///////////////////////////////////////////////////////////////////////////////
//      MOUSE POSITIONING
///////////////////////////////////////////////////////////////////////////////
void input::setMousePos( int x, int y ) {
    glfwSetMousePos( x, y );
}

void input::getMousePos( int* x, int* y ) {
    glfwGetMousePos( x, y );
}

void input::setMouseWheelPos( int p ) {
    glfwSetMouseWheel( p );
}

void input::getMouseWheelPos( int* p ) {
    *p = glfwGetMouseWheel();
}
