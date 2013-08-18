
#include "renderer.h"

/*
 * Exceptions are used during construction in order to catch any loaded resources
 * and remove them before they become memory leaks
 */

namespace hge {

/******************************************************************************
 * Deferred Renderer
******************************************************************************/
/*
 * Initialization and Termination
 */
bool dsRenderer::init( const vec2i& resolution ) {
    try {
        HL_ASSERT( pipeline::init() );
        /*
         * Deferred rendering
         */
        pGBuffer = new( std::nothrow ) gBuffer();
        HL_ASSERT( pGBuffer != nullptr );
        HL_ASSERT( pGBuffer->init( resolution ) );

        /*
         * Shaders
         */
        pPlainShader = new( std::nothrow ) plainShader();
        HL_ASSERT( pPlainShader != nullptr );
        HL_ASSERT( pPlainShader->init() );

        pGeomShader = new( std::nothrow ) dsGeometryShader();
        HL_ASSERT( pGeomShader != nullptr );
        HL_ASSERT( pGeomShader->init() );

        pDsLightShader = new( std::nothrow ) dsLightShader();
        HL_ASSERT( pDsLightShader != nullptr );
        HL_ASSERT( pDsLightShader->init( resolution ) );

        pNullShader = new( std::nothrow ) dsNullShader();
        HL_ASSERT( pNullShader != nullptr );
        HL_ASSERT( pNullShader->init() );

        pSkyShader = new( std::nothrow ) skyShader;
        HL_ASSERT( pSkyShader != nullptr );
        HL_ASSERT( pSkyShader->init() );

        pFontShader = new( std::nothrow ) fontShader();
        HL_ASSERT( pFontShader != nullptr );
        HL_ASSERT( pFontShader->init() );

        pBillboardShader = new( std::nothrow ) billboardShader();
        HL_ASSERT( pBillboardShader != nullptr );
        HL_ASSERT( pBillboardShader->init() );
        
#ifdef DEBUG
        pEnbtShader = new( std::nothrow ) enbtShader();
        HL_ASSERT( pEnbtShader != nullptr );
        HL_ASSERT( pEnbtShader->init() );
#endif
        HL_ASSERT( lightSphere.init() );
    }
    catch( const hamLibs::utils::errorType e ) {
        terminate();
        return false;
    }
    return true;
}

void dsRenderer::terminate() {
    lightSphere.terminate();
    dsPointLights.clear();
    
    delete pGBuffer;
    pGBuffer        = nullptr;
    delete pPlainShader;
    pPlainShader     = nullptr;
    delete pGeomShader;
    pGeomShader      = nullptr;
    delete pDsLightShader;
    pDsLightShader  = nullptr;
    delete pNullShader;
    pNullShader     = nullptr;
    delete pSkyShader;
    pSkyShader      = nullptr;
    delete pFontShader;
    pFontShader     = nullptr;
    delete pBillboardShader;
    pBillboardShader = nullptr;
    
#ifdef DEBUG
    delete pEnbtShader;
    pEnbtShader = nullptr;
#endif
    
    pipeline::terminate();
}

/*
 * Renderer Main Operation
 */
void dsRenderer::tick() {
    appTimer.update();
    
    updateScene( appTimer.getTickDuration() );
    
    doGeometryPass();
    
    glEnable( GL_STENCIL_TEST );
    doStencilPass();
    doLightingPass();
    glDisable( GL_STENCIL_TEST );
    applyStockShader( pPlainShader->getProgramId() );
    drawSceneUnlit();
#ifdef DEBUG
    //doNbtPass();
#endif
    doBillboardPass();
    doSkyPass();
    doFontPass();
    
    pGBuffer->bindForReading();
    pGBuffer->drawBuffer();
}

/*
 * GEOMETRY PASS
 */
void dsRenderer::doGeometryPass() {
    // Tell the GBuffer to prepare the next frame
    applyStockShader( pGeomShader->getProgramId() );
    pGBuffer->bindForWriting();
    
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
    
    pGBuffer->bindForGeometryPass();
    
    drawSceneLit();
}

/*
 * STENCIL BUFFER PASS
 */
void dsRenderer::doStencilPass() {
    // Disable color and depth writes to the stencil buffer
    applyStockShader( pNullShader->getProgramId() );
    pGBuffer->bindForStencilPass();
    
    glDepthMask( GL_FALSE ); // don't allow the stencil pass to update the depth buffer
    glDisable( GL_CULL_FACE );
    
    // always pass the stencil test
    glStencilFunc( GL_ALWAYS, 0, 0 );
    
    glStencilOpSeparate( GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP );
    glStencilOpSeparate( GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP );
    
    lightSphere.draw();
    
    glEnable( GL_CULL_FACE );
    glDepthMask( GL_TRUE );
}

/*
 * LIGHTING PASS
 */
void dsRenderer::doLightingPass() {
    applyStockShader( pDsLightShader->getProgramId() );
    pGBuffer->bindForLightPass();
    
    glStencilFunc( GL_NOTEQUAL, 0, 0xFF );
    
    glDisable( GL_DEPTH_TEST );
    
    glEnable( GL_BLEND );
    glBlendEquation( GL_FUNC_ADD );
    glBlendFunc( GL_ONE, GL_ONE );
    
    glCullFace( GL_FRONT );
    
    lightSphere.draw();
    
    glCullFace( GL_BACK );
    
    glDisable( GL_BLEND );
    
    glEnable( GL_DEPTH_TEST );
}

/*
 * RESOLUTION HANDLING
 */
void dsRenderer::setResolution( const vec2i& res ) {
    pGBuffer->setBufferResolution( res );
    
    applyStockShader( pDsLightShader->getProgramId() );
    pDsLightShader->setInputResolution( res );
    
    changeResolution( res );
}

/*
 * LIGHTING LAUNCHING
 */
void dsRenderer::launchPointLight( const dsPointLight& l ) {
    dsPointLights.push_back         ( l );
    lightSphere.setLightBuffer    ( dsPointLights.data(), dsPointLights.size() );
}

/*
 * FONT HANDLING
 */
void dsRenderer::setFontColor( const vec4& c ) {
    applyStockShader ( pFontShader->getProgramId() );
    pFontShader->setFontColor   ( c );
}

/******************************************************************************
 * Forward Renderer
******************************************************************************/
/*
 * Initialization & termination
 */
bool fwdRenderer::init() {
    try {
        HL_ASSERT( pipeline::init() );
        /*
         * Shaders
         */
        pPlainShader = new( std::nothrow ) plainShader();
        HL_ASSERT( pPlainShader != nullptr );
        HL_ASSERT( pPlainShader->init() );

        pPointLightShader = new( std::nothrow ) pointLightShader();
        HL_ASSERT( pPointLightShader != nullptr );
        HL_ASSERT( pPointLightShader->init() );

        pSkyShader = new( std::nothrow ) skyShader;
        HL_ASSERT( pSkyShader != nullptr );
        HL_ASSERT( pSkyShader->init() );

        pFontShader = new( std::nothrow ) fontShader();
        HL_ASSERT( pFontShader != nullptr );
        HL_ASSERT( pFontShader->init() );

        pBillboardShader = new( std::nothrow ) billboardShader();
        HL_ASSERT( pBillboardShader != nullptr );
        HL_ASSERT( pBillboardShader->init() );

#ifdef DEBUG
        pEnbtShader = new( std::nothrow ) enbtShader();
        HL_ASSERT( pEnbtShader != nullptr );
        HL_ASSERT( pEnbtShader->init() );
        pEnbtShader->showEdges( false );
        pEnbtShader->showNormals( false );
        pEnbtShader->showTangents( false );
        pEnbtShader->showBitangents( false );
#endif
    }
    catch( const hamLibs::utils::errorType& e ) {
        terminate();
        return false;
    }
    return true;
}

void fwdRenderer::terminate() {
    delete pPlainShader;
    pPlainShader = nullptr;
    delete pPointLightShader;
    pPointLightShader = nullptr;
    delete pSkyShader;
    pSkyShader = nullptr;
    delete pFontShader;
    pFontShader = nullptr;
    delete pBillboardShader;
    pBillboardShader = nullptr;
    
#ifdef DEBUG
    delete pEnbtShader;
    pEnbtShader = nullptr;
#endif
    
    pipeline::terminate();
}

/*
 * Renderer Main Operation
 */
void fwdRenderer::tick() {
    appTimer.update();
    
    updateScene( appTimer.getTickDuration() );
    
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    doGeometryPass();
    doLightingPass();
    doBillboardPass();
#ifdef DEBUG
    //doNbtPass();
#endif
    doSkyPass();
    doFontPass();
}

/*
 * LIGHT HANDLING
 */
void fwdRenderer::removePointLight() {
    pntLight.color        = vec4( 0.f, 0.f, 0.f, 1.f );
    pntLight.intensity    = 0.f;
    pntLight.constant     = 0.0f;
    pntLight.exponential  = 0.0f;
    pntLight.linear       = 0.0f;
    pntLight.pos          = vec3( 0.f );
    
    applyStockShader( pPointLightShader->getProgramId() );
    pPointLightShader->setPointLight( pntLight );
}

void fwdRenderer::removeAmbientLight() {
    ambLight.color        = vec4( 0.f, 0.f, 0.f, 1.f );
    ambLight.intensity    = 0.f;
    
    applyStockShader( pPointLightShader->getProgramId() );
    pPointLightShader->setAmbientLight( ambLight );
}

} // end hge namespace
