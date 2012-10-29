/* 
 * File:   harbinger.h
 * Author: hammy
 *
 * Created on September 8, 2012, 3:29 PM
 */

#ifndef	__HGE_HARBINGER_H__
#define	__HGE_HARBINGER_H__

#ifndef __cplusplus
	#error "A C++ compiler is required to build the Harbinger Game Engine"
#endif

#include <cassert>
#include <stdexcept>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <list>
#include <queue>
#include <glm/glm.hpp>
#include <allegro5/allegro.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "../../hamlibs/include/hamLibs.h"

#ifndef NULL
	#define NULL 0x0
#endif

//forward declarations
#include "types.h"

namespace harbinger {
	bool init();
} // end harbinger namespace

#include "scripting/script_base.h"
#include "scripting/script_variables.h"
#include "scripting/script_functions.h"

#include "events/message.h"

#include "utils/clock.h"
#include "utils/serialize.h"

#include "obj/resource.h"
#include "obj/object.h"

#endif	/* __HGE_HARBINGER_H__ */
