
#include <GLFW/glfw3.h>
#include "input.h"

using namespace hge;

///////////////////////////////////////////////////////////////////////////////
//      INPUT CONSTRUCTION
///////////////////////////////////////////////////////////////////////////////
inputHandler::inputHandler( window& w ) {
    pContext = w.getContext();
}

inputHandler::inputHandler( inputHandler&& i ) {
    HGE_ASSERT( i.pContext != nullptr );
    pContext = i.pContext;
    i.pContext = nullptr;
}

inputHandler& inputHandler::operator = ( inputHandler&& i ) {
    HGE_ASSERT( i.pContext != nullptr );
    pContext = i.pContext;
    i.pContext = nullptr;
}

///////////////////////////////////////////////////////////////////////////////
//      INPUT CALLBACKS
///////////////////////////////////////////////////////////////////////////////
void inputHandler::setKeyCallback( void(*func)( window::context*, key_t,int, action_t, mod_t ) ) {
    glfwSetKeyCallback( pContext, (GLFWkeyfun)func );
}

void inputHandler::setTextCallback( void(*func)( window::context*,int ) ) {
    glfwSetCharCallback( pContext, (GLFWcharfun)func );
}

void inputHandler::setMouseCallback( void(*func)( window::context*, mouse_t, action_t, mod_t ) ) {
    glfwSetMouseButtonCallback( pContext, (GLFWmousebuttonfun)func );
}

void inputHandler::setMousePosCallback( void(*func)( window::context*,double,double ) ) {
    glfwSetCursorPosCallback( pContext, (GLFWcursorposfun)func );
}

void inputHandler::setMouseWheelCallback( void(*func)( window::context*,double,double ) ) {
    glfwSetScrollCallback( pContext, (GLFWscrollfun)func );
}

///////////////////////////////////////////////////////////////////////////////
//      BUTTON PRESSES
///////////////////////////////////////////////////////////////////////////////
inputHandler::action_t inputHandler::getKeyButtonState( inputHandler::key_t k ) {
    return (inputHandler::action_t)glfwGetKey( pContext, k );
}

inputHandler::action_t inputHandler::getMouseButtonState( inputHandler::mouse_t m ) {
    return (inputHandler::action_t)glfwGetMouseButton( pContext, m );
}

///////////////////////////////////////////////////////////////////////////////
//      MOUSE POSITIONING
///////////////////////////////////////////////////////////////////////////////
void inputHandler::setMousePos( const vec2d& pos ) {
    glfwSetCursorPos( pContext, pos.index.x, pos.index.y );
}

vec2d inputHandler::getMousePos() {
    vec2d pos;
    glfwGetCursorPos( pContext, &pos.index.x, &pos.index.y );
    return pos;
}
