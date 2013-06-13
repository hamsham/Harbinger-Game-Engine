
#include <GL/glfw3.h>
#include "input.h"

using namespace hge;

extern GLFWwindow* mainWindow;

///////////////////////////////////////////////////////////////////////////////
//      INPUT CALLBACKS
///////////////////////////////////////////////////////////////////////////////
void input::setKeyCallback( hgeWindow* w, void(*func)(hgeWindow*,key_t,int,action_t,mod_t) ) {
    glfwSetKeyCallback( w, (GLFWkeyfun)func );
}

void input::setTextCallback( hgeWindow* w, void(*func)(hgeWindow*,unsigned) ) {
    glfwSetCharCallback( w, (GLFWcharfun)func );
}

void input::setMouseCallback( hgeWindow* w, void(*func)(hgeWindow*,mouse_t,action_t,mod_t) ) {
    glfwSetMouseButtonCallback( w, (GLFWmousebuttonfun)func );
}

void input::setMousePosCallback( hgeWindow* w, void(*func)(hgeWindow*,double, double) ) {
    glfwSetCursorPosCallback( w, (GLFWcursorposfun)func );
}

void input::setMouseWheelCallback( hgeWindow* w, void(*func)(hgeWindow*,double, double) ) {
    glfwSetScrollCallback( w, (GLFWscrollfun)func );
}

///////////////////////////////////////////////////////////////////////////////
//      BUTTON PRESSES
///////////////////////////////////////////////////////////////////////////////
bool input::isKeyPressed( int keycode ) {
    return glfwGetKey( display::getCurrentWindow(), keycode ) == GLFW_PRESS;
}

bool input::isMouseButtonPressed( int button ) {
    return glfwGetMouseButton( display::getCurrentWindow(), button ) == GLFW_PRESS;
}

///////////////////////////////////////////////////////////////////////////////
//      MOUSE POSITIONING
///////////////////////////////////////////////////////////////////////////////
void input::setMousePos( double x, double y ) {
    glfwSetCursorPos( mainWindow, x, y );
}

void input::getMousePos( double* x, double* y ) {
    glfwGetCursorPos( mainWindow, x, y );
}
