/* 
 * File:   shaderMgr.h
 * Author: hammy
 *
 * Created on May 14, 2013, 12:25 AM
 */

#ifndef __HGE_STOCK_SHADERS_H__
#define	__HGE_STOCK_SHADERS_H__

#include "types.h"

namespace hge {
namespace stockShaders {

    enum e_matrixState : int {
        HGE_MODEL_MAT   = 0,
        HGE_VP_MAT      = 1
    };
    
    HGE_API bool    init                ();
    HGE_API void    terminate           ();
    
    HGE_API void    applyShader( GLuint shaderId );

    // The matrix stack will only hold (at most) a single user-applied matrix of
    // each type (model, view, and projection).
    HGE_API void    applyMatrix         ( e_matrixState s, const mat4& m = mat4( 1.f ) );
    HGE_API void    applyMatrix         ( const drawTransform&, const camera& );
    HGE_API void    removeMatrix        ( e_matrixState s );
    
    HGE_API void    applyBillboardShader();
    HGE_API void    setBillboardCam     ( const camera& );
    
    HGE_API void    applyPointLightShader();
    HGE_API void    setPointLight       ( const ambientLight&, const pointLight& );
    HGE_API void    setPointLightMvp    ( const mat4& );
    
    HGE_API void    applyShadowShader   ();
    
    HGE_API void    applyFontShader     ();
    HGE_API void    setFontColor        ( const vec4& );
    
    HGE_API void    applySkyShader      ();
    
} // stock shaders namespace
} // end hge namespace

#endif	/* __HGE_STOCK_SHADERS_H__ */

