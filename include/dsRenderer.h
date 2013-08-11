/* 
 * File:   application.h
 * Author: hammy
 *
 * Created on June 2, 2013, 10:54 PM
 */

#ifndef __HGE_DEFERRED_RENDERER_H__
#define	__HGE_DEFERRED_RENDERER_H__

#include <vector>
#include "types.h"
#include "pipeline.h"
#include "application.h"
#include "stockShaders.h"
#include "light.h"
#include "gBuffer.h"

namespace hge {

/******************************************************************************
 * Rendering Application Class
******************************************************************************/
class HGE_API dsRenderer : public hge::application, public hge::pipeline {
    
    private:
        hge::dsGeometryShader* pGeoShader   = nullptr; // Shader to draw geometry
        hge::dsLightShader* pDsLightShader  = nullptr; // point light shader
        hge::dsNullShader*  pNullShader     = nullptr; // stencil buffer shader
        hge::skyShader*     pSkyShader      = nullptr; // sky box
        hge::fontShader*    pFontShader     = nullptr; // fonts
        hge::gBuffer*       pGBuffer        = nullptr;
        
    protected:
        std::vector< hge::dsPointLight > dsPointLights;
        hge::dsLightSphere  lightSphere;

    public:
        dsRenderer          ( const vec2i& resolution );
        dsRenderer          () : dsRenderer( vec2i(0,0) ) {}
        dsRenderer          ( const dsRenderer& ) = delete;
        dsRenderer          ( dsRenderer&& )      = delete;
        
        virtual ~dsRenderer ();
        
        dsRenderer&         operator =          ( const dsRenderer& )    = delete;
        dsRenderer&         operator =          ( dsRenderer&& )         = delete;
        
        virtual bool        init                () = 0;
        virtual void        terminate           () = 0;
        
        virtual void        tick                () final;
        
        // resolution handling
        void                setResolution       ( const vec2i& );
        
        // Light handling
        void                launchPointLight    ( const hge::dsPointLight& );
        void                removePointLight    ( unsigned i );
        unsigned            getNumPointLights   () const;
        void                clearPointLights    ();
        const hge::dsPointLight& getPointLight       ( unsigned i ) const;
        void                setPointLight       ( unsigned i, const hge::dsPointLight& );
        
        // font handling
        void                setFontColor        ( const vec4& );
        
    protected:
        virtual void        updateScene         ( float ) = 0;
        
        virtual void        changeResolution    ( const vec2i& ) {}
        
        virtual void        drawScene           () = 0;
        virtual void        drawSky             () = 0;
        virtual void        drawFonts           () = 0;
        
    private:
        void doGeometryPass (); // calls "drawScene()"
        void doStencilPass  (); // draws all of the point lights to the stencil buffer
        void doLightingPass (); // draws all visible point lights
        void doSkyPass      ();
        void doFontPass     ();
        
#ifdef DEBUG
        hge::enbtShader*    pEnbtShader = nullptr;
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

inline const hge::dsPointLight& dsRenderer::getPointLight( unsigned i ) const {
    return dsPointLights[ i ];
}

inline void dsRenderer::setPointLight( unsigned i, const hge::dsPointLight& l ) {
    dsPointLights[ i ] = l;
}

/******************************************************************************
 * ENBT
******************************************************************************/
#ifdef DEBUG
inline void dsRenderer::doNbtPass() {
    applyStockShader( pEnbtShader->getProgramId() );
    drawScene();
    drawFonts();
}
#endif

/******************************************************************************
 * SKYBOX
******************************************************************************/
inline void dsRenderer::doSkyPass() {
    applyStockShader( pSkyShader->getProgramId() );
    drawSky();
}

/******************************************************************************
 * DRAW FONTS
******************************************************************************/
inline void dsRenderer::doFontPass() {
    applyStockShader( pFontShader->getProgramId() );
    drawFonts();
}

} // end hge namespace

#endif	/* __DEFERRED_RENDERER_H__ */

