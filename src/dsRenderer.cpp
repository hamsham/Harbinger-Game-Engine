
#include "dsRenderer.h"

namespace hge {

/******************************************************************************
 * Renderer Construction & Destruction
******************************************************************************/
dsRenderer::dsRenderer( const vec2i& resolution ) {
    HGE_ASSERT( hge::pipeline::init() );
    /*
     * Deferred rendering
     */
    pGBuffer = new( std::nothrow ) hge::gBuffer();
    HL_ASSERT( pGBuffer != nullptr );
    HL_ASSERT( pGBuffer->init( resolution ) );

    /*
     * Shaders
     */
    pGeoShader = new( std::nothrow ) hge::dsGeometryShader();
    HL_ASSERT( pGeoShader != nullptr );
    HL_ASSERT( pGeoShader->init() );

    pDsLightShader = new( std::nothrow ) hge::dsLightShader();
    HL_ASSERT( pDsLightShader != nullptr );
    HL_ASSERT( pDsLightShader->init( resolution ) );

    pNullShader = new( std::nothrow ) hge::dsNullShader();
    HL_ASSERT( pNullShader != nullptr );
    HL_ASSERT( pNullShader->init() );

    pSkyShader = new( std::nothrow ) hge::skyShader;
    HL_ASSERT( pSkyShader != nullptr );
    HL_ASSERT( pSkyShader->init() );

    pFontShader = new( std::nothrow ) hge::fontShader();
    HL_ASSERT( pFontShader != nullptr );
    HL_ASSERT( pFontShader->init() );

#ifdef DEBUG
    pEnbtShader = new( std::nothrow ) hge::enbtShader();
    HL_ASSERT( pEnbtShader != nullptr );
    HL_ASSERT( pEnbtShader->init() );
#endif
}

dsRenderer::~dsRenderer() {
    lightSphere.terminate();
    dsPointLights.clear();
    
    delete pGBuffer;
    pGBuffer        = nullptr;
    delete pGeoShader;
    pGeoShader      = nullptr;
    delete pDsLightShader;
    pDsLightShader  = nullptr;
    delete pNullShader;
    pNullShader     = nullptr;
    delete pSkyShader;
    pSkyShader      = nullptr;
    delete pFontShader;
    pFontShader     = nullptr;
    
#ifdef DEBUG
    delete pEnbtShader;
    pEnbtShader = nullptr;
#endif
}

/******************************************************************************
 * Renderer Main Operation
******************************************************************************/
void dsRenderer::tick() {
    appTimer.update();
    
    updateScene( appTimer.getTickDuration() );
    
    doGeometryPass();
    
    glEnable( GL_STENCIL_TEST );
    doStencilPass();
    doLightingPass();
    glDisable( GL_STENCIL_TEST );
    
#ifdef DEBUG
    //doNbtPass();
#endif
    doSkyPass();
    doFontPass();
    
    pGBuffer->bindForReading();
    pGBuffer->drawBuffer();
}

/******************************************************************************
 * GEOMETRY PASS
******************************************************************************/
void dsRenderer::doGeometryPass() {
    // Tell the GBuffer to prepare the next frame
    applyStockShader( pGeoShader->getProgramId() );
    pGBuffer->bindForWriting();
    
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
    
    pGBuffer->bindForGeometryPass();
    
    drawScene();
}

/******************************************************************************
 * STENCIL BUFFER PASS
******************************************************************************/
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

/******************************************************************************
 * LIGHTING PASS
******************************************************************************/
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

/******************************************************************************
 * RESOLUTION HANDLING
******************************************************************************/
void dsRenderer::setResolution( const vec2i& res ) {
    pGBuffer->setBufferResolution( res );
    
    applyStockShader( pDsLightShader->getProgramId() );
    pDsLightShader->setInputResolution( res );
    
    changeResolution( res );
}

/******************************************************************************
 * LIGHTING LAUNCHING
******************************************************************************/
void dsRenderer::launchPointLight( const hge::dsPointLight& l ) {
    dsPointLights.push_back         ( l );
    lightSphere.setLightBuffer    ( dsPointLights.data(), dsPointLights.size() );
}

/******************************************************************************
 * FONT HANDLING
******************************************************************************/
void dsRenderer::setFontColor( const vec4& c ) {
    applyStockShader ( pFontShader->getProgramId() );
    pFontShader->setFontColor   ( c );
}

} // end hge namespace
