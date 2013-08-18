/* 
 * File:   application.h
 * Author: hammy
 *
 * Created on June 2, 2013, 10:54 PM
 */

#ifndef __HGE_DEFERRED_RENDERER_H__
#define	__HGE_DEFERRED_RENDERER_H__

#include <vector>
#include "pipeline.h"
#include "stockShaders.h"
#include "light.h"
#include "gBuffer.h"

namespace hge {

/******************************************************************************
 * Deferred Rendering Class
******************************************************************************/
class HGE_API dsRenderer : public pipeline {
    
    private:
        gBuffer*            pGBuffer        = nullptr;
        dsGeometryShader*   pGeomShader     = nullptr; // Shader to draw geometry
        dsLightShader*      pDsLightShader  = nullptr; // point light shader
        dsNullShader*       pNullShader     = nullptr; // stencil buffer shader
        skyShader*          pSkyShader      = nullptr; // sky box
        fontShader*         pFontShader     = nullptr; // fonts
        billboardShader*    pBillboardShader = nullptr;
        
    protected:
        std::vector< dsPointLight > dsPointLights;
        dsLightSphere  lightSphere;

    public:
        dsRenderer          () {}
        dsRenderer          ( const dsRenderer& ) = delete;
        dsRenderer          ( dsRenderer&& )      = delete;
        
        virtual ~dsRenderer () { dsRenderer::terminate(); } // ensure that inherited functions aren't called
        
        dsRenderer&         operator =          ( const dsRenderer& )    = delete;
        dsRenderer&         operator =          ( dsRenderer&& )         = delete;
        
        virtual bool        init                ( const vec2i& resolution );
        virtual bool        init                () override { return init( vec2i(0,0) ); }
        virtual void        terminate           () override;
        void                tick                () final;
        
        // resolution handling
        void                setResolution       ( const vec2i& ) final;
        
        // Light handling
        void                launchPointLight    ( const dsPointLight& );
        void                removePointLight    ( unsigned i );
        unsigned            getNumPointLights   () const;
        void                clearPointLights    ();
        const dsPointLight& getPointLight       ( unsigned i ) const;
        void                setPointLight       ( unsigned i, const dsPointLight& );
        
        // font handling
        void                setFontColor        ( const vec4& );
        // billboard handling
        void                setBillboardTarget  ( const vec3& );
        
    protected:
        virtual void        updateScene         ( float ) = 0;
        virtual void        changeResolution    ( const vec2i& res ) = 0;
        virtual void        drawSceneLit        () = 0;
        virtual void        drawSceneUnlit      () = 0;
        virtual void        drawSky             () = 0;
        virtual void        drawFonts           () = 0;
        virtual void        drawBillboards      () = 0;
        
    private:
        void doGeometryPass () final; // calls "drawSceneLit()"
        void doStencilPass  (); // draws all of the point lights to the stencil buffer
        void doLightingPass () final; // draws all visible point lights
        void doSkyPass      () final;
        void doFontPass     () final;
        void doBillboardPass() final;
        
#ifdef DEBUG
        enbtShader*    pEnbtShader = nullptr;
        void doNbtPass      ();
        
    public:
        void showEdges      ( bool b ) { pEnbtShader->showEdges(b); }
        void showNormals    ( bool b ) { pEnbtShader->showNormals(b); }
        void showTangents   ( bool b ) { pEnbtShader->showTangents(b); }
        void showBitangents ( bool b ) { pEnbtShader->showBitangents(b); }
#endif
};

inline void dsRenderer::removePointLight( unsigned i ) {
    dsPointLights.erase( dsPointLights.begin() + i );
}

inline unsigned dsRenderer::getNumPointLights() const {
    return dsPointLights.size();
}

inline void dsRenderer::clearPointLights() {
    dsPointLights.clear();
    lightSphere.setLightBuffer( nullptr, 0 );
}

inline const dsPointLight& dsRenderer::getPointLight( unsigned i ) const {
    return dsPointLights[ i ];
}

inline void dsRenderer::setPointLight( unsigned i, const dsPointLight& l ) {
    dsPointLights[ i ] = l;
}

/*
 * ENBT
 */
#ifdef DEBUG
inline void dsRenderer::doNbtPass() {
    applyStockShader( pEnbtShader->getProgramId() );
    drawSceneLit();
    drawSceneUnlit();
    drawFonts();
}
#endif

/*
 * SKYBOX
 */
inline void dsRenderer::doSkyPass() {
    applyStockShader( pSkyShader->getProgramId() );
    drawSky();
}

/*
 * DRAW FONTS
 */
inline void dsRenderer::doFontPass() {
    applyStockShader( pFontShader->getProgramId() );
    drawFonts();
}

/*
 * Billboards
 */
inline void dsRenderer::setBillboardTarget( const vec3& target ) {
    pBillboardShader->setTargetPos( target );
}

inline void dsRenderer::doBillboardPass() {
    applyStockShader( pBillboardShader->getProgramId() );
    drawBillboards();
}

/******************************************************************************
 * Forward Rendering Class
******************************************************************************/
class HGE_API fwdRenderer : public pipeline {
    
    private:
        plainShader*        pPlainShader        = nullptr;
        pointLightShader*   pPointLightShader   = nullptr; // Shader to draw geometry
        skyShader*          pSkyShader          = nullptr; // sky box
        fontShader*         pFontShader         = nullptr; // fonts
        billboardShader*    pBillboardShader    = nullptr;
        
    protected:
        std::vector< pointLight > pointLights;

    public:
        fwdRenderer         () {}
        fwdRenderer         ( const fwdRenderer& ) = delete;
        fwdRenderer         ( fwdRenderer&& )      = delete;
        
        virtual ~fwdRenderer() { fwdRenderer::terminate(); }
        
        fwdRenderer&        operator =          ( const fwdRenderer& )    = delete;
        fwdRenderer&        operator =          ( fwdRenderer&& )         = delete;
        
        virtual bool        init                () override;
        virtual void        terminate           () override;
        void                tick                () final;
        
        // resolution handling
        void                setResolution       ( const vec2i& res ) final { changeResolution( res ); }
        
        // Light handling
        void                launchPointLight    ( const pointLight& );
        void                removePointLight    ( unsigned i );
        unsigned            getNumPointLights   () const;
        void                clearPointLights    ();
        const pointLight&   getPointLight       ( unsigned i ) const;
        void                setPointLight       ( unsigned i, const pointLight& );
        void                setAmbientLight     ( const ambientLight& );
        
        // font handling
        void                setFontColor        ( const vec4& ) final;
        // billboard handling
        void                setBillboardTarget  ( const vec3& ) final;
        
    protected:
        virtual void        updateScene         ( float ) = 0;
        virtual void        changeResolution    ( const vec2i& ) = 0;
        virtual void        drawSceneLit        () = 0;
        virtual void        drawSceneUnlit      () = 0;
        virtual void        drawSky             () = 0;
        virtual void        drawFonts           () = 0;
        virtual void        drawBillboards      () = 0;
        
    private:
        void doGeometryPass () final; // calls "drawSceneUnlit()"
        void doLightingPass () final; // calls "drawSceneLit();
        void doSkyPass      () final;
        void doFontPass     () final;
        void doBillboardPass() final;
        
#ifdef DEBUG
        enbtShader*         pEnbtShader = nullptr;
        void doNbtPass      ();
        
    public:
        void showEdges      ( bool b ) { pEnbtShader->showEdges(b); }
        void showNormals    ( bool b ) { pEnbtShader->showNormals(b); }
        void showTangents   ( bool b ) { pEnbtShader->showTangents(b); }
        void showBitangents ( bool b ) { pEnbtShader->showBitangents(b); }
#endif
};

/*
 * LIGHT HANDLING
 */
inline void fwdRenderer::launchPointLight( const pointLight& l ) {
    pointLights.push_back( l );
}

inline void fwdRenderer::removePointLight( unsigned i ) {
    pointLights.erase( pointLights.begin() + i );
    if ( !pointLights.size() )
        clearPointLights();
}

inline unsigned fwdRenderer::getNumPointLights() const {
    return pointLights.size();
}

inline const pointLight& fwdRenderer::getPointLight( unsigned i ) const {
    HL_ASSERT( i < pointLights.size() );
    return pointLights[ i ];
}

inline void fwdRenderer::setPointLight( unsigned i, const pointLight& l ) {
    HL_ASSERT( i < pointLights.size() );
    applyStockShader( pPointLightShader->getProgramId() );
    pointLights[ i ] = l;
}

inline void fwdRenderer::setAmbientLight( const ambientLight& l ) {
    applyStockShader( pPointLightShader->getProgramId() );
    pPointLightShader->setAmbientLight( l );
}

/*
 * GEOMETRY
 */
inline void fwdRenderer::doGeometryPass() {
    applyStockShader( pPlainShader->getProgramId() );
    drawSceneUnlit(); 
}

/*
 * Billboards
 */
inline void fwdRenderer::setBillboardTarget( const vec3& target ) {
    pBillboardShader->setTargetPos( target );
}

inline void fwdRenderer::doBillboardPass() {
    applyStockShader( pBillboardShader->getProgramId() );
    drawBillboards();
}

/*
 * ENBT
 */
#ifdef DEBUG
inline void fwdRenderer::doNbtPass() {
    applyStockShader( pEnbtShader->getProgramId() );
    drawSceneUnlit();
    drawSceneLit();
    drawFonts();
}
#endif

/*
 * SKYBOX
 */
inline void fwdRenderer::doSkyPass() {
    applyStockShader( pSkyShader->getProgramId() );
    drawSky();
}

/*
 * FONT HANDLING
 */
inline void fwdRenderer::setFontColor( const vec4& c ) {
    applyStockShader( pFontShader->getProgramId() );
    pFontShader->setFontColor( c );
}

inline void fwdRenderer::doFontPass() {
    applyStockShader( pFontShader->getProgramId() );
    drawFonts();
}

} // end hge namespace

#endif	/* __DEFERRED_RENDERER_H__ */

