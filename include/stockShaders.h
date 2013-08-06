/* 
 * File:   shaderMgr.h
 * Author: hammy
 *
 * Created on May 14, 2013, 12:25 AM
 */

#ifndef __HGE_STOCK_SHADERS_H__
#define	__HGE_STOCK_SHADERS_H__

#include <string>
#include "types.h"

namespace hge {
    
class HGE_API stockShaders {
    private:
        //Point Light Shader
        hge::shader     pointLightShader;
        GLint           ambColorId      = 0;
        GLint           ambIntId        = 0;
        GLint           pointColorId    = 0;
        GLint           pointIntId      = 0;
        GLint           pointLinearId   = 0;
        GLint           pointConstId    = 0;
        GLint           pointExpId      = 0;
        GLint           pointPosId      = 0;
        GLint           shadowId        = 0;
        GLint           lightMatrixId   = 0;
        GLint           textureId       = 0;
        GLint           normalId        = 0;
        // Shadow Shader
        hge::shader     shadowShader;
        GLint           shadowMapId     = 0;
        // Skybox Shader
        hge::shader     skyShader;
        GLint           skyTexId        = 0;
        // Font Shader Setup
        hge::shader     fontShader;
        GLint           fontColId       = 0;
        GLint           fontSampler     = 0;
        // Billboard Setup
        hge::shader     bbShader;
        GLint           camPosId        = 0;
        GLint           bbTexSampler    = 0;
        // Normal/Tangent/Bitangent visualizer
        hge::shader     nbtShader;
        GLint           nbtShowVertId   = 0;
        GLint           nbtShowNormId   = 0;
        GLint           nbtShowTangId   = 0;
        GLint           nbtShowBtngId   = 0;

        // Commonly shared Uniform buffer object
        GLuint ubo = 0;
        GLuint matrixIndexId = GL_INVALID_INDEX;
        GLuint currShader = 0;

        // Creating an array of four 4x4 Matrices. Model, View, Projection, and MVP
        mat4 transforms[ 3 ] = { mat4( 1.f ) };
        
        // Shader initializations
        bool initPointLightShader();
        bool initShadowShader();
        bool initSkyShader();
        bool initFontShader();
        bool initBillboardShader();
        bool initNbtShader();
        void terminate();
        
        // Shader Matrix Updating
        void updateMatricesImpl();
        
    public:
        enum e_matrixState : int {
            HGE_MODEL_MAT   = 0,
            HGE_VP_MAT      = 1
        };

        stockShaders();
        stockShaders( const stockShaders& ) = delete;
        stockShaders( stockShaders&& ) = delete;
        
        stockShaders& operator = ( const stockShaders& ) = delete;
        stockShaders& operator = ( stockShaders&& ) = delete;
        
        ~stockShaders() { terminate(); }

        void    applyShader( GLuint shaderId );

        // The matrix stack will only hold (at most) a single user-applied matrix of
        // each type (model, view, and projection).
        void    applyMatrix         ( e_matrixState s, const mat4& m = mat4( 1.f ) );
        void    applyMatrix         ( const drawTransform&, const camera& );
        void    removeMatrix        ( e_matrixState s );

        void    applyBillboardShader();
        void    setBillboardCam     ( const camera& );

        void    applyPointLightShader();
        void    setPointLight       ( const ambientLight&, const pointLight& );
        void    setPointLightMvp    ( const mat4& );

        void    applyShadowShader   ();

        void    applyFontShader     ();
        void    setFontColor        ( const vec4& );

        void    applySkyShader      ();
        
        // The NBT shader only works with objects that have been drawn using
        // the GL_TRIANGLE* specifiers. Objects drawn with GL_LINES, GL_POINTS,
        // or the GL_*_ADJACENCY specifiers will not work
        void    applyNbtShader      ();
        void    showVertices        ( bool );
        void    showNormals         ( bool );
        void    showTangents        ( bool );
        void    showBitangents      ( bool );
        
};

} // end hge namespace

#endif	/* __HGE_STOCK_SHADERS_H__ */

