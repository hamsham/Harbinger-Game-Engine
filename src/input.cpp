
#include <GLFW/glfw3.h>
#include "input.h"

using namespace hge;

///////////////////////////////////////////////////////////////////////////////
//      INPUT CALLBACKS
///////////////////////////////////////////////////////////////////////////////
void input::setKeyCallback(
    display::hgeWindow* win,
    void(*func)( display::hgeWindow*,input::key_t,int,input::action_t,input::mod_t )
) {
    glfwSetKeyCallback( win, (GLFWkeyfun)func );
}

void input::setTextCallback(
    display::hgeWindow* w,
    void(*func)( display::hgeWindow*,int )
) {
    glfwSetCharCallback( w, (GLFWcharfun)func );
}

void input::setMouseCallback(
    display::hgeWindow* w,
    void(*func)( display::hgeWindow*,input::mouse_t,input::action_t,input::mod_t )
) {
    glfwSetMouseButtonCallback( w, (GLFWmousebuttonfun)func );
}

void input::setMousePosCallback(
    display::hgeWindow* w,
    void(*func)( display::hgeWindow*,double,double )
) {
    glfwSetCursorPosCallback( w, (GLFWcursorposfun)func );
}

void input::setMouseWheelCallback(
    display::hgeWindow* w,
    void(*func)( display::hgeWindow*,double,double )
) {
    glfwSetScrollCallback( w, (GLFWscrollfun)func );
}

///////////////////////////////////////////////////////////////////////////////
//      BUTTON PRESSES
///////////////////////////////////////////////////////////////////////////////
input::action_t input::getKeyButtonState( display::hgeWindow* w, input::key_t k ) {
    return (input::action_t)glfwGetKey( w, k );
}

input::action_t input::getMouseButtonState( display::hgeWindow* w, input::mouse_t m ) {
    return (input::action_t)glfwGetMouseButton( w, m );
}

///////////////////////////////////////////////////////////////////////////////
//      MOUSE POSITIONING
///////////////////////////////////////////////////////////////////////////////
void input::setMousePos( display::hgeWindow* w, const vec2d& pos ) {
    glfwSetCursorPos( w, pos.index.x, pos.index.y );
}

vec2d input::getMousePos( display::hgeWindow* w ) {
    vec2d pos;
    glfwGetCursorPos( w, &pos.index.x, &pos.index.y );
    return pos;
}
