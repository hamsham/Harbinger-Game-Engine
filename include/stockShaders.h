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
    
    HGE_API bool    init                ();
    HGE_API void    terminate           ();
    
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

