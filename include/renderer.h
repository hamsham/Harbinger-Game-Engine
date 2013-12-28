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

/*
 * NOTE:
 *  Multiple lights with the forward renderer is not currently working. Only one
 * light can be used to draw a scene at a time.
 * Deferred shading can use multiple lights, however.
 */

namespace hge {

/******************************************************************************
 * Deferred Rendering Class
******************************************************************************/
class HGE_API dsRenderer : virtual public pipeline {
    
    private:
        gBuffer*            pGBuffer        = nullptr;
        plainShader*        pPlainShader    = nullptr;
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
        virtual bool        init                () override { return init( vec2i(HGE_DEFAULT_WINDOW_WIDTH, HGE_DEFAULT_WINDOW_HEIGHT) ); }
        virtual void        terminate           () override;
        void                tick                () final;
        
        // resolution handling
        void                setResolution       ( const vec2i& ) final;
        
        // Light handling
        void                addPointLight       ( const dsPointLight& );
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
        virtual void        drawSceneLit        () {}
        virtual void        drawSceneUnlit      () {}
        virtual void        drawSky             () {}
        virtual void        drawFonts           () {}
        virtual void        drawBillboards      () {}
        virtual void        drawDebug           () {}
        
    private:
        void doGeometryPass () final; // calls "drawSceneLit()"
        void doStencilPass  (); // draws all of the point lights to the stencil buffer
        void doLightingPass () final; // draws all visible point lights
        void doSkyPass      () final;
        void doFontPass     () final;
        void doBillboardPass() final;
        
#ifdef DEBUG
        enbtShader*         pEnbtShader = nullptr;
        void doNbtPass      ();
        
    public:
        void showEdges      ( bool );
        void showNormals    ( bool );
        void showTangents   ( bool );
        void showBitangents ( bool );
#else
    public:
        void showEdges      ( bool ) {}
        void showNormals    ( bool ) {}
        void showTangents   ( bool ) {}
        void showBitangents ( bool ) {}
#endif
};

/*
 * LIGHTING LAUNCHING
 */
inline void dsRenderer::addPointLight( const dsPointLight& l ) {
    dsPointLights.push_back( l );
    lightSphere.setLightBuffer( dsPointLights.data(), dsPointLights.size() );
}

inline void dsRenderer::removePointLight( unsigned i ) {
    HL_ASSERT( i < dsPointLights.size() );
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
    HL_ASSERT( i < dsPointLights.size() );
    return dsPointLights[ i ];
}

inline void dsRenderer::setPointLight( unsigned i, const dsPointLight& l ) {
    HL_ASSERT( i < dsPointLights.size() );
    dsPointLights[ i ] = l;
    lightSphere.setLight( i, l );
}

/*
 * ENBT
 */
#ifdef DEBUG
inline void dsRenderer::doNbtPass() {
    applyStockShader( pEnbtShader->getProgramId() );
    drawDebug();
}

inline void dsRenderer::showEdges( bool b ) {
    applyStockShader( pEnbtShader->getProgramId() );
    pEnbtShader->showEdges(b);
}

inline void dsRenderer::showNormals( bool b ) {
    applyStockShader( pEnbtShader->getProgramId() );
    pEnbtShader->showNormals(b);
}

inline void dsRenderer::showTangents( bool b ) {
    applyStockShader( pEnbtShader->getProgramId() );
    pEnbtShader->showTangents(b);
}

inline void dsRenderer::showBitangents( bool b ) {
    applyStockShader( pEnbtShader->getProgramId() );
    pEnbtShader->showBitangents(b);
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
    glEnable( GL_BLEND );
    applyStockShader( pFontShader->getProgramId() );
    drawFonts();
    glDisable( GL_BLEND );
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
class HGE_API fwdRenderer : virtual public pipeline {
    
    private:
        plainShader*        pPlainShader        = nullptr;
        pointLightShader*   pPointLightShader   = nullptr; // Shader to draw geometry
        skyShader*          pSkyShader          = nullptr; // sky box
        fontShader*         pFontShader         = nullptr; // fonts
        billboardShader*    pBillboardShader    = nullptr;
        
    protected:
        ambientLight        ambLight;
        pointLight          pntLight;

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
        void                removePointLight    ();
        const pointLight&   getPointLight       () const { return pntLight; }
        void                setPointLight       ( const pointLight& pl );
        void                removeAmbientLight  ();
        const ambientLight& getAmbientLight     () const { return ambLight; }
        void                setAmbientLight     ( const ambientLight& a );
        
        // font handling
        void                setFontColor        ( const vec4& ) final;
        // billboard handling
        void                setBillboardTarget  ( const vec3& ) final;
        
    protected:
        virtual void        updateScene         ( float ) = 0;
        virtual void        changeResolution    ( const vec2i& ) = 0;
        virtual void        drawSceneLit        () {}
        virtual void        drawSceneUnlit      () {}
        virtual void        drawSky             () {}
        virtual void        drawFonts           () {}
        virtual void        drawBillboards      () {}
        virtual void        drawDebug           () {}
        
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
        void showEdges      ( bool );
        void showNormals    ( bool );
        void showTangents   ( bool );
        void showBitangents ( bool );
#else
    public:
        void showEdges      ( bool ) {}
        void showNormals    ( bool ) {}
        void showTangents   ( bool ) {}
        void showBitangents ( bool ) {}
#endif
};

/*
 * LIGHT HANDLING
 */
inline void fwdRenderer::setPointLight( const pointLight& l ) {
    applyStockShader( pPointLightShader->getProgramId() );
    pntLight = l;
    pPointLightShader->setPointLight( pntLight );
}

inline void fwdRenderer::setAmbientLight( const ambientLight& l ) {
    applyStockShader( pPointLightShader->getProgramId() );
    ambLight = l;
    pPointLightShader->setAmbientLight( ambLight );
}

/*
 * LIGHTING PASS
 */
inline void fwdRenderer::doLightingPass() {
    applyStockShader( pPointLightShader->getProgramId() );
    drawSceneLit();
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
    drawDebug();
}

inline void fwdRenderer::showEdges( bool b ) {
    applyStockShader( pEnbtShader->getProgramId() );
    pEnbtShader->showEdges(b);
}

inline void fwdRenderer::showNormals( bool b ) {
    applyStockShader( pEnbtShader->getProgramId() );
    pEnbtShader->showNormals(b);
}

inline void fwdRenderer::showTangents( bool b ) {
    applyStockShader( pEnbtShader->getProgramId() );
    pEnbtShader->showTangents(b);
}

inline void fwdRenderer::showBitangents( bool b ) {
    applyStockShader( pEnbtShader->getProgramId() );
    pEnbtShader->showBitangents(b);
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

