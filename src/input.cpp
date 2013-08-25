
#include <GLFW/glfw3.h>
#include "input.h"

using namespace hge;

///////////////////////////////////////////////////////////////////////////////
//      INPUT CONSTRUCTION
///////////////////////////////////////////////////////////////////////////////
input::input( window& w ) {
    HGE_ASSERT( pContext = w.getContext() );
}

input::input( input&& i ) {
    HGE_ASSERT( i.pContext != nullptr );
    pContext = i.pContext;
    
    pKeyButtonCB    = i.pKeyButtonCB;
    pKeyTextCB      = i.pKeyTextCB;
    pMouseButtonCB  = i.pMouseButtonCB;
    pMousePosCB     = i.pMousePosCB;
    pMouseWheelCB   = i.pMouseWheelCB;
    
    i.pKeyButtonCB  = nullptr;
    i.pKeyTextCB    = nullptr;
    i.pMouseButtonCB= nullptr;
    i.pMousePosCB   = nullptr;
    i.pMouseWheelCB = nullptr;
}

input& input::operator = ( input&& i ) {
    HGE_ASSERT( i.pContext != nullptr );
    pContext = i.pContext;
    
    pKeyButtonCB    = i.pKeyButtonCB;
    pKeyTextCB      = i.pKeyTextCB;
    pMouseButtonCB  = i.pMouseButtonCB;
    pMousePosCB     = i.pMousePosCB;
    pMouseWheelCB   = i.pMouseWheelCB;
    
    i.pKeyButtonCB  = nullptr;
    i.pKeyTextCB    = nullptr;
    i.pMouseButtonCB= nullptr;
    i.pMousePosCB   = nullptr;
    i.pMouseWheelCB = nullptr;
}

input::~input() {
    pContext        = nullptr;
    pKeyButtonCB    = nullptr;
    pKeyTextCB      = nullptr;
    pMouseButtonCB  = nullptr;
    pMousePosCB     = nullptr;
    pMouseWheelCB   = nullptr;
}

///////////////////////////////////////////////////////////////////////////////
//      INPUT CALLBACKS
///////////////////////////////////////////////////////////////////////////////
void input::applyCallbacks() {
    glfwSetKeyCallback          ( pContext, (GLFWkeyfun)pKeyButtonCB );
    glfwSetCharCallback         ( pContext, (GLFWcharfun)pKeyTextCB );
    glfwSetMouseButtonCallback  ( pContext, (GLFWmousebuttonfun)pMouseButtonCB );
    glfwSetCursorPosCallback    ( pContext, (GLFWcursorposfun)pMousePosCB );
    glfwSetScrollCallback       ( pContext, (GLFWscrollfun)pMouseWheelCB );
}

///////////////////////////////////////////////////////////////////////////////
//      BUTTON PRESSES
///////////////////////////////////////////////////////////////////////////////
input::action_t input::getKeyButtonState( input::key_t k ) {
    return (input::action_t)glfwGetKey( pContext, k );
}

input::action_t input::getMouseButtonState( input::mouse_t m ) {
    return (input::action_t)glfwGetMouseButton( pContext, m );
}

///////////////////////////////////////////////////////////////////////////////
//      MOUSE POSITIONING
///////////////////////////////////////////////////////////////////////////////
void input::setMousePos( const vec2d& pos ) {
    glfwSetCursorPos( pContext, pos.index.x, pos.index.y );
}

vec2d input::getMousePos() {
    vec2d pos;
    glfwGetCursorPos( pContext, &pos.index.x, &pos.index.y );
    return pos;
}

void input::showCursor( bool cursor ) {
    if ( cursor )
        glfwSetInputMode( pContext, GLFW_CURSOR, GLFW_CURSOR_NORMAL );
    else
        glfwSetInputMode(pContext, GLFW_CURSOR, GLFW_CURSOR_HIDDEN );
}
