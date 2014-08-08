
#include <iostream>

#include "main.h"
#include "renderApp.h"
#include "logicApp.h"

/*
 * GLOBAL VARIABLES
 */
hge::dsRenderer*    pRenderer   = nullptr;
gameLogic*          pLogic      = nullptr;
//hge::pipeline*  pPipeline   = nullptr;

/*
 * FUNCTION DECLARATIONS
 */
unsigned    tickI       ();
bool        init        ();
void        terminate   ();

/******************************************************************************
 * Program initialization
******************************************************************************/
bool init() {

    if ( !hge::init() ) {
        std::cerr
            << "An error occurred while initializing and creating a window"
            << std::endl;
        return false;
    }

    if ( !gameSystem::init() ) {
        std::cerr
            << "An error occurred while initializing and creating a window"
            << std::endl;
        return false;
    }
    
    try {
        HL_ASSERT( (pLogic = new( std::nothrow ) gameLogic()) );
        HL_ASSERT( (pRenderer = new( std::nothrow ) renderApp()) );
        HL_ASSERT( pRenderer->init( gameSystem::getSystemWindow().getResolution() ) );
    }
    catch( hamLibs::utils::error_t e ) {
        std::cerr
            << "ERROR: Unable to initialize global variables. "
            << "Please review all initialization procedures before running "
            << "the program again.\n\tStatus code: " << (int)e
            << std::endl;
        return false;
    }
    return true;
}

/******************************************************************************
 * Main
******************************************************************************/
int main() {
    
    if ( !init() ) {
        terminate();
        return -1;
    }
    
    hge::window& pWindow = gameSystem::getSystemWindow();
    
    pLogic->startRunning();
    pRenderer->startRunning();
    
    while ( pLogic->isRunning() && !pWindow.wantsToClose() ) {
        pLogic->tick();
        pRenderer->tick();
        pWindow.flip();
    }
    
    pRenderer->stopRunning();
    
    printGlError("Post main-loop error");
    
    terminate();
    
    return 0;
}

/******************************************************************************
 * Main
******************************************************************************/
void terminate() {
    //delete pPipeline;
    //pPipeline = nullptr;
    
    delete pRenderer;
    pRenderer = nullptr;
    
    delete pLogic;
    pLogic = nullptr;
    
    gameSystem::terminate();
    hge::terminate();
    
    std::cout
        << "Program terminated successfully.\n"
        << "Run time:\t" << utils::clock::progUpTime()
        << std::endl;
}

//-----------------------------------------------------------------------------
//	Integer FPS Counter
//-----------------------------------------------------------------------------
unsigned tickI() {
    static unsigned retVal( 0 ), numFrames( 0 );
    static time_t prevTime( 0 );
    time_t currTime = std::time(NULL);

    ++numFrames;
    if (currTime - prevTime >= 1) {
        retVal = numFrames;
        prevTime = currTime;
        numFrames = 0;
    }

    return retVal;
}
