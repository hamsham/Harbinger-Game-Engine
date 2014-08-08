/* 
 * File:   renderApp.cpp
 * Author: hammy
 * 
 * Created on July 29, 2013, 4:02 PM
 */

#include <iostream>
#include "renderApp.h"
#include "logicApp.h"
#include "blankNormalMap.h"

const char* skyTextures[6] = {
    "resources/sky/sky_r.jpg",
    "resources/sky/sky_l.jpg",
    "resources/sky/sky_d.jpg",
    "resources/sky/sky_t.jpg",
    "resources/sky/sky_f.jpg",
    "resources/sky/sky_b.jpg"
};

/******************************************************************************
 * Initializations
******************************************************************************/
bool renderApp::init( const math::vec2i& res ) {
    if ( !hge::dsRenderer::init( res ) ) return false;
    
    /*
     * Resources
     */
    if ( !testPrim.init( 1 ) ) return false;
    //testMesh.load( "resources/sponza.obj" );
    if ( !testMesh.load( "resources/sponza.obj" ) ) return false;
    if ( !testCube.init() ) return false;
    if ( !lightSphere.init() ) return false;
    
    if ( !floorTex.load( "resources/brick_diffuse.jpg" ) ) return false;
    if ( !normalMap.load( "resources/brick_normal.jpg" ) ) return false;
    
    if ( !sky.load( skyTextures ) ) return false;
    
    if ( !testFont.load( "resources/testfont.ttf", testFontSize ) ) return false;
    
    if ( !testNormalMap.init() ) return false;
    
    /*
     * Object setup
     */
    rndGen.seed( (unsigned long)(appTimer.globalSecond() + appTimer.globalMinute()) );
    
    mainCam.lockYAxis( true );
    mainCam.look( math::vec3( 0.f, 0.f, 1.f ), math::vec3( 0.f ), math::vec3( 0.f, 1.f, 0.f ) );
    
    mainCam.setProjectionParams( 90.f, res[0], res[1], 0.1f, 100.f );
    overlayCam.setProjectionParams( 45.f, res[0], res[1], -1.f, 1.f );
    mainCam.makePerspective     ();
    overlayCam.makeOrtho        ();
    mainCam.update              ();
    overlayCam.update           ();
    
    glViewport                  ( 0, 0, res[0], res[1] );
    
    /*
     * 3D Positioning of 2D text
     */
    setFontColor                ( math::vec4( 0.f, 1.f, 0.f, 0.5f ) );
    
    fpsTrans.setScale           ( math::vec3( 0.025f ) );
    fpsTrans.setPos             ( math::vec3( -10.f, 2.f, -10.f ) );
    posTrans.setPos             ( math::vec3( -1200.f, 600.f, 0.f ) );
    fpsTrans.update             ();
    posTrans.update             ();
    posString.setString         ( testFont, "Harbinger Game Engine" );
    
    /*
     * Billboards
     */
    floorTex.setTexUnit         ( hge::pipeline::HGE_TEXTURE_DIFFUSE );
    normalMap.setTexUnit        ( hge::pipeline::HGE_TEXTURE_NORMAL );
    
    /*
     * 3D Meshes and Primitives
     */
    meshTrans.setPos            ( math::vec3( 0.f, 0.f, 0.f ) );
    meshTrans.setScaleUniform   ( 0.1f );
    //meshTrans.rotate            ( math::vec3( 0.f, -HL_PI_OVER_2, 0.f ) );
    meshTrans.update            ();
    
    //floorTrans.setPos           ( math::vec3( 0.f, -10.f, 0.f ) );
    primTrans.setPos            ( math::vec3( 0.f, 0.f, 0.f ) );
    primTrans.setScaleUniform   ( 5.f );
    primTrans.update            ();
    
    resetLights();
    
    showEdges( true );
    showNormals( true );
    showTangents( true );
    showBitangents( true );
    
    return true;
}

/******************************************************************************
 * Termination processes
******************************************************************************/
void renderApp::terminate() {
    hge::dsRenderer::terminate();
}

/******************************************************************************
 * Main Loop
******************************************************************************/
void renderApp::updateScene( float timeElapsed ) {
    static int prevTime;
    int currTime = tickI();
    
    mainCam.update();
    applyMatrix( HGE_VP_MAT, mainCam.getVPMatrix() );
    
    if ( prevTime != currTime ) {
        std::string&& currTimeElapsedStr
            = "fps: " + to_string( currTime ) + "\nms: " + to_string( timeElapsed );

        fpsString.setString( testFont, currTimeElapsedStr.c_str() );
        
        prevTime = currTime;
    }
    
    primTrans.rotate( math::vec3( timeElapsed, 0.f, 0.f ) );
    primTrans.update();
}

/******************************************************************************
 * MAIN SCENE DRAWING
******************************************************************************/
void renderApp::drawSceneLit() {
    applyMatrix( HGE_MODEL_MAT, meshTrans.getModelMatrix() );
    testNormalMap.activate();
    testMesh.draw();
    
    applyMatrix( HGE_MODEL_MAT, primTrans.getModelMatrix() );
    normalMap.activate();
    floorTex.activate();
    testPrim.draw();
    floorTex.deActivate();
    normalMap.deActivate();     
}

/******************************************************************************
 * SKYBOX
******************************************************************************/
void renderApp::drawSky() {
    skyTrans.setPos( mainCam.getPos() );
    skyTrans.update();
    
    applyMatrix( HGE_MODEL_MAT, skyTrans.getModelMatrix() );
    sky.draw();
}

/******************************************************************************
 * DRAW FONTS
******************************************************************************/
void renderApp::drawFonts() {
    
    testFont.activate();
    
    applyMatrix( posTrans, overlayCam );
    fpsString.draw();
    
    applyMatrix( fpsTrans, mainCam );
    posString.draw();
    
    testFont.deActivate();
}

/******************************************************************************
 * LIGHTING LAUNCHING
******************************************************************************/
void renderApp::launchRandLight() {
    dsPointLights.push_back( hge::dsPointLight() );
    hge::dsPointLight& tempLight = dsPointLights.back();
    
    const math::vec4 colors = math::vec4(
        rndGen.randRangeF( 0.f, 1.f ),
        rndGen.randRangeF( 0.f, 1.f ),
        rndGen.randRangeF( 0.f, 1.f ),
        1.f
    );
    
    tempLight.position      = mainCam.getPos();
    tempLight.color         = colors;
    tempLight.attrib.intensity = 100.f;
    tempLight.scale         = tempLight.calcInfluenceRadius();
    
    std::cout
        << "Launched light " << dsPointLights.size() << " with colors "
        << colors[0] << ' ' << colors[1] << ' ' << colors[2] << std::endl;
    
    lightSphere.setLightBuffer( dsPointLights.data(), dsPointLights.size() );
}
/******************************************************************************
 * RESET LIGHTING
******************************************************************************/
void renderApp::resetLights() {
    dsPointLights.clear();
    lightSphere.setLightBuffer( nullptr, 0 );
    
    /*
     * Initialize a point light
     */
    dsPointLights.push_back( hge::dsPointLight() );
    hge::dsPointLight& tempLight = dsPointLights[0];
    
    tempLight.position      = math::vec3( 0.f, 10.f, 0.f );
    tempLight.color         = math::vec4( 1.f );
    tempLight.attributes[2] = 100.f;
    
    tempLight.scale         = tempLight.calcInfluenceRadius();
    
    for ( int i = -3; i < 3; ++i ) {
        for ( int j = -3; j < 3; ++j ) {
            for ( int k = -3; k < 3; ++k ) {
                dsPointLights.push_back( hge::dsPointLight() );
                hge::dsPointLight& tempLight = dsPointLights.back();

                const math::vec4 colors = math::vec4(
                    rndGen.randRangeF( 0.f, 1.f ),
                    rndGen.randRangeF( 0.f, 1.f ),
                    rndGen.randRangeF( 0.f, 1.f ),
                    1.f
                );

                tempLight.position      = math::vec3( (float)i * 20.f, (float)j * 30.f, (float)k * 20.f );
                tempLight.color         = colors;
                tempLight.attrib.intensity = 50.f;
                tempLight.scale         = tempLight.calcInfluenceRadius();
            }
        }
    }
    
    lightSphere.setLightBuffer( dsPointLights.data(), dsPointLights.size() );
}