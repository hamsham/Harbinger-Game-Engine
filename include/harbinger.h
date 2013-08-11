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

#include "application.h"
#include "billboard.h"
#include "bitmap.h"
#include "camera.h"
#include "display.h"
#include "drawable.h"
#include "dsRenderer.h"
#include "font.h"
#include "gBuffer.h"
#include "geometry.h"
#include "input.h"
#include "light.h"
#include "mesh.h"
#include "pipeline.h"
#include "primitives.h"
#include "resource.h"
#include "shader.h"
#include "shadowMap.h"
#include "skybox.h"
#include "stockShaders.h"
#include "text.h"
#include "transformations.h"

namespace hge {

HGE_API bool    init();
HGE_API void    terminate();

} // end hge namespace

#endif	/* __HARBINGER_H__ */

