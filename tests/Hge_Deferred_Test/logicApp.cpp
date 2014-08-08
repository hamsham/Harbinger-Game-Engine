
//input.cpp

#include <new> /* used for std::nothrow */
#include <iostream>

#include "logicApp.h"
#include "renderApp.h"

//using namespace hge;
#define HGE_RELEASED  hge::input::HGE_RELEASED
#define HGE_PRESSED   hge::input::HGE_PRESSED
#define HGE_REPEATED  hge::input::HGE_REPEATED

/******************************************************************************
 * LOW-LEVEL GAME SYSTEMS
******************************************************************************/
//-----------------------------------------------------------------------------
//	Static Input Members
//-----------------------------------------------------------------------------
math::vec2d gameSystem::mouseVel          = math::vec2d( 1.f );

hge::window*        gameSystem::pWindow   = nullptr;
hge::input*   gameSystem::pInputs   = nullptr;

//-----------------------------------------------------------------------------
//	Game System Initialization
//-----------------------------------------------------------------------------
bool gameSystem::init() {
    if ( pWindow )
        return true;
    
    try {
        pWindow = new( std::nothrow ) hge::window( 1280, 720, false, nullptr, false );
        pInputs = new( std::nothrow ) hge::input( *pWindow );
    }
    catch( hamLibs::utils::error_t e ) {
        std::cerr
            << "ERROR: Unable to initialize the window and input systems. "
            << "Please review all initialization procedures before running "
            << "the program again.\n\tStatus code: " << (int)e
            << std::endl;
        terminate();
        return false;
    }
    
    return true;
}

//-----------------------------------------------------------------------------
//	Game System Termination
//-----------------------------------------------------------------------------
void gameSystem::terminate() {
    delete pWindow;
    pWindow = nullptr;
    
    delete pInputs;
    pInputs = nullptr;
    
    mouseVel = math::vec2d( 1.f );
}

/******************************************************************************
 * GAME LOGIC
******************************************************************************/
bool gameLogic::showVNBT[ 4 ] = { false,false,false,false };

void gameLogic::startRunning() {
    hge::input& pInputs = gameSystem::getSystemInputs();
    
    pInputs.setKeyButtonCB( &keyboardCallback );
    pInputs.setMouseButtonCB( &mouseButtonCallback );
    pInputs.setMousePosCB( &mousePosCallback );
    pInputs.setKeyTextCB( nullptr );
    pInputs.setMouseWheelCB( nullptr );
    pInputs.applyCallbacks();
    
    application::startRunning();
}

void gameLogic::stopRunning() {
    hge::input& pInputs = gameSystem::getSystemInputs();
    
    pInputs.setKeyButtonCB( nullptr );
    pInputs.setMouseButtonCB( nullptr );
    pInputs.setMousePosCB( nullptr );
    pInputs.setKeyTextCB( nullptr );
    pInputs.setMouseWheelCB( nullptr );
    pInputs.applyCallbacks();
    
    application::stopRunning();
}

//-----------------------------------------------------------------------------
//	Mouse Updating Callback Function
//-----------------------------------------------------------------------------
void gameLogic::mousePosCallback( hge::window::context*, double mx, double my ) {
    hge::input& pInputs = gameSystem::getSystemInputs();
    
    const math::vec2i& res      = gameSystem::getSystemWindow().getResolution();
    const math::vec2d& halfRes  = math::vec2d( (double)res[0]/2, (double)res[1]/2 );
    const math::vec2d& deltaPos = halfRes - math::vec2d( mx, my );
    
    pInputs.setMousePos( halfRes );
    
    static_cast< renderApp* >( pRenderer )->mainCam.rotate(
        math::vec3( deltaPos[1],deltaPos[0],0.0 ) * CAM_ROTATE_SPEED
    );
}

//-----------------------------------------------------------------------------
//	Mouse Button Callback Function
//-----------------------------------------------------------------------------
void gameLogic::mouseButtonCallback( hge::window::context*, mouse_t mb, action_t a, mod_t ) {
    if ( mb == hge::input::HGE_MB_LEFT && a == HGE_PRESSED ) {
        static_cast< renderApp* >( pRenderer )->launchRandLight();
    }
    else if ( mb == hge::input::HGE_MB_RIGHT && a == HGE_PRESSED ) {
        static_cast< renderApp* >( pRenderer )->resetLights();
    }
}

//-----------------------------------------------------------------------------
//	Keyboard Button Callback Function
//-----------------------------------------------------------------------------
void gameLogic::keyboardCallback( hge::window::context*, key_t k, int, action_t a, mod_t ) {
    if ( a == HGE_RELEASED )
        return;
    
    renderApp* const pRenderApp = static_cast< renderApp* >( pRenderer );

    /*
     * CAMERA CONTROL TYPES
     */
    const math::vec2i& res = gameSystem::getSystemWindow().getResolution();
    switch( k ) {
        case key_t::HGE_KEY_SPACE:
            pRenderApp->mainCam.look( math::vec3( 0.f, 15.f, 20.f ), math::vec3( 0.f ), math::vec3( 0.f, 1.f, 0.f ) );
            pRenderApp->mainCam.unroll();
            break;
            
        //MESH MANIPULATION
        case key_t::HGE_KEY_T:
            pRenderApp->mainCam.setProjectionParams( 45.f, res[0], res[1], 0.1f, 100.f );
            pRenderApp->mainCam.makePerspective();
            break;
            
        case key_t::HGE_KEY_Y:
            pRenderApp->mainCam.setProjectionParams( 45.f, 16.f, 9.f, 0.1f, 100.f );
            pRenderApp->mainCam.makeOrtho();
            break;
            
        // Debug Drawing of VNBT's
#ifdef DEBUG
        case key_t::HGE_KEY_1:
            showVNBT[ 0 ] = !showVNBT[ 0 ];
            pRenderApp->showEdges( showVNBT[ 0 ] );
            break;
        case key_t::HGE_KEY_2:
            showVNBT[ 1 ] = !showVNBT[ 1 ];
            pRenderApp->showNormals( showVNBT[ 1 ] );
            break;
        case key_t::HGE_KEY_3:
            showVNBT[ 2 ] = !showVNBT[ 2 ];
            pRenderApp->showTangents( showVNBT[ 2 ] );
            break;
        case key_t::HGE_KEY_4:
            showVNBT[ 3 ] = !showVNBT[ 3 ];
            pRenderApp->showBitangents( showVNBT[ 3 ] );
            break;
#endif
            
        // PROGRAM SHUT-DOWN
        case key_t::HGE_KEY_ESCAPE:
            pLogic->stopRunning();
            break;
            
        default:
            break;
    }
}
	
//-----------------------------------------------------------------------------
//	Updating user input
//-----------------------------------------------------------------------------
void gameLogic::tick() {
    appTimer.update();
    
    hge::input& pInputs = gameSystem::getSystemInputs();
    const float timeStep = CAM_MOVE_SPEED * appTimer.getTickDuration();
    
    /*
     * MOVEMENT
     */
    math::vec3 pos( 0.f, 0.f, 0.f );

    if ( pInputs.getKeyButtonState( key_t::HGE_KEY_W ) == HGE_PRESSED ) {
        pos[2] = timeStep * CAM_MOVE_SPEED;
    }

    if ( pInputs.getKeyButtonState( key_t::HGE_KEY_A ) == HGE_PRESSED ) {
        pos[0] = timeStep * CAM_MOVE_SPEED;
    }

    if ( pInputs.getKeyButtonState( key_t::HGE_KEY_S ) == HGE_PRESSED ) {
        pos[2] = -timeStep * CAM_MOVE_SPEED;
    }

    if ( pInputs.getKeyButtonState( key_t::HGE_KEY_D ) == HGE_PRESSED ) {
        pos[0] = -timeStep * CAM_MOVE_SPEED;
    }

    static_cast< renderApp* >( pRenderer )->mainCam.move( pos );
}
