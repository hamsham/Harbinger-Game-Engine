/* 
 * File:   renderApp.h
 * Author: hammy
 *
 * Created on July 29, 2013, 4:02 PM
 */

#ifndef __RENDERAPP_H__
#define	__RENDERAPP_H__

#include <vector>
#include "utils/randomNum.h"

#include "main.h"
#include "blankNormalMap.h"

/******************************************************************************
 * Game Application Class
******************************************************************************/
class renderApp final : virtual public hge::pipeline, public hge::dsRenderer {
    friend class gameLogic;
    
    private:
        const char* const   progTitle       = "Harbinger Test Executable";
        const int           testFontSize    = 72;
        utils::randomNum    rndGen;
        
        hge::skybox         sky;
        hge::sphere         testPrim;
        
        hge::mesh           testMesh;
        hge::cube           testCube;
        hge::text           fpsString;
        hge::text           posString;
        
        hge::bitmap         floorTex;
        hge::bitmap         normalMap;
        hge::font           testFont;
        blankNormalMap      testNormalMap;
        
        hge::drawTransform  fpsTrans;
        hge::drawTransform  posTrans;
        hge::drawTransform  meshTrans;
        hge::drawTransform  primTrans;
        hge::drawTransform  skyTrans;
        
        // Renderer viewports
        hge::camera         mainCam;
        hge::camera         overlayCam;

    public:
        renderApp                 ( const renderApp& )    = delete;
        renderApp                 ( renderApp&& )         = delete;
        renderApp& operator =     ( const renderApp& )    = delete;
        renderApp& operator =     ( renderApp&& )         = delete;
        
        renderApp                 () {}
        ~renderApp                () { terminate(); }
        
    private:
        bool    init            () { return init( math::vec2i(0,0) ); }
        bool    init            ( const math::vec2i& res );
        void    updateScene     ( float );
        void    terminate       ();
        
        void    drawSceneLit    ();
        void    drawSceneUnlit  () {}
        void    drawSky         ();
        void    drawFonts       ();
        void    drawDebug       () { /*drawSceneLit();*/ }
        
        void changeResolution( const math::vec2i& ) {}
        void drawBillboards() {}
        
        void    launchRandLight ();
        void    resetLights     ();
};

#endif	/* RENDERAPP_H */

