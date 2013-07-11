
#include <GLFW/glfw3.h>
#include "input.h"

using namespace hge;

///////////////////////////////////////////////////////////////////////////////
//      INPUT CALLBACKS
///////////////////////////////////////////////////////////////////////////////
void input::setKeyCallback(
    window::context* win,
    void(*func)( window::context*,input::key_t,int,input::action_t,input::mod_t )
) {
    glfwSetKeyCallback( win, (GLFWkeyfun)func );
}

void input::setTextCallback(
    window::context* w,
    void(*func)( window::context*,int )
) {
    glfwSetCharCallback( w, (GLFWcharfun)func );
}

void input::setMouseCallback(
    window::context* w,
    void(*func)( window::context*,input::mouse_t,input::action_t,input::mod_t )
) {
    glfwSetMouseButtonCallback( w, (GLFWmousebuttonfun)func );
}

void input::setMousePosCallback(
    window::context* w,
    void(*func)( window::context*,double,double )
) {
    glfwSetCursorPosCallback( w, (GLFWcursorposfun)func );
}

void input::setMouseWheelCallback(
    window::context* w,
    void(*func)( window::context*,double,double )
) {
    glfwSetScrollCallback( w, (GLFWscrollfun)func );
}

///////////////////////////////////////////////////////////////////////////////
//      BUTTON PRESSES
///////////////////////////////////////////////////////////////////////////////
input::action_t input::getKeyButtonState( window::context* w, input::key_t k ) {
    return (input::action_t)glfwGetKey( w, k );
}

input::action_t input::getMouseButtonState( window::context* w, input::mouse_t m ) {
    return (input::action_t)glfwGetMouseButton( w, m );
}

///////////////////////////////////////////////////////////////////////////////
//      MOUSE POSITIONING
///////////////////////////////////////////////////////////////////////////////
void input::setMousePos( window::context* w, const vec2d& pos ) {
    glfwSetCursorPos( w, pos.index.x, pos.index.y );
}

vec2d input::getMousePos( window::context* w ) {
    vec2d pos;
    glfwGetCursorPos( w, &pos.index.x, &pos.index.y );
    return pos;
}
