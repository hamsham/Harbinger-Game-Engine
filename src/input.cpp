
#include <GLFW/glfw3.h>
#include "input.h"

using namespace hge;

///////////////////////////////////////////////////////////////////////////////
//      INPUT CONSTRUCTION
///////////////////////////////////////////////////////////////////////////////
inputSystem::inputSystem( window& w ) {
    HGE_ASSERT( pContext = w.getContext() );
}

inputSystem::inputSystem( inputSystem&& i ) {
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

inputSystem& inputSystem::operator = ( inputSystem&& i ) {
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

inputSystem::~inputSystem() {
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
void inputSystem::applyCallbacks() {
    glfwSetKeyCallback          ( pContext, (GLFWkeyfun)pKeyButtonCB );
    glfwSetCharCallback         ( pContext, (GLFWcharfun)pKeyTextCB );
    glfwSetMouseButtonCallback  ( pContext, (GLFWmousebuttonfun)pMouseButtonCB );
    glfwSetCursorPosCallback    ( pContext, (GLFWcursorposfun)pMousePosCB );
    glfwSetScrollCallback       ( pContext, (GLFWscrollfun)pMouseWheelCB );
}

///////////////////////////////////////////////////////////////////////////////
//      BUTTON PRESSES
///////////////////////////////////////////////////////////////////////////////
inputSystem::action_t inputSystem::getKeyButtonState( inputSystem::key_t k ) {
    return (inputSystem::action_t)glfwGetKey( pContext, k );
}

inputSystem::action_t inputSystem::getMouseButtonState( inputSystem::mouse_t m ) {
    return (inputSystem::action_t)glfwGetMouseButton( pContext, m );
}

///////////////////////////////////////////////////////////////////////////////
//      MOUSE POSITIONING
///////////////////////////////////////////////////////////////////////////////
void inputSystem::setMousePos( const vec2d& pos ) {
    glfwSetCursorPos( pContext, pos.index.x, pos.index.y );
}

vec2d inputSystem::getMousePos() {
    vec2d pos;
    glfwGetCursorPos( pContext, &pos.index.x, &pos.index.y );
    return pos;
}

void inputSystem::showCursor( bool cursor ) {
    if ( cursor )
        glfwSetInputMode( pContext, GLFW_CURSOR, GLFW_CURSOR_NORMAL );
    else
        glfwSetInputMode(pContext, GLFW_CURSOR, GLFW_CURSOR_HIDDEN );
}
