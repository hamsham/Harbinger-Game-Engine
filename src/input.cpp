
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
void inputHandler::setKeyCallback( keyboardCallback_t func ) {
    glfwSetKeyCallback( pContext, (GLFWkeyfun)func );
}

void inputHandler::setTextCallback( textCallback_t func ) {
    glfwSetCharCallback( pContext, (GLFWcharfun)func );
}

void inputHandler::setMouseCallback( mouseCallback_t func ) {
    glfwSetMouseButtonCallback( pContext, (GLFWmousebuttonfun)func );
}

void inputHandler::setMousePosCallback( mousePosCallback_t func ) {
    glfwSetCursorPosCallback( pContext, (GLFWcursorposfun)func );
}

void inputHandler::setMouseWheelCallback( mouseWheelCallback_t func ) {
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

void inputHandler::showCursor( bool cursor ) {
    if ( cursor )
        glfwSetInputMode( pContext, GLFW_CURSOR, GLFW_CURSOR_NORMAL );
    else
        glfwSetInputMode(pContext, GLFW_CURSOR, GLFW_CURSOR_HIDDEN );
}
