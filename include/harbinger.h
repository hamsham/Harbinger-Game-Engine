/* 
 * File:   harbinger.h
 * Author: hammy
 *
 * Created on March 19, 2013, 7:53 PM
 */

#ifndef __HARBINGER_H__
#define	__HARBINGER_H__

#include "setup.h"
#include "types.h"

#include "pipeline.h"
#include "display.h"
#include "input.h"
#include "resource.h"
#include "object.h"
#include "shader.h"
#include "camera.h"
#include "mesh.h"
#include "light.h"
#include "bitmap.h"
#include "image.h"
#include "timeObject.h"
#include "font.h"

namespace hge {

#ifdef __cplusplus
    extern "C" {
#endif
    
    HGE_API bool    init();
    HGE_API void    terminate();
    
#ifdef __cplusplus
    }
#endif
} // end hge namespace

#endif	/* __HARBINGER_H__ */

