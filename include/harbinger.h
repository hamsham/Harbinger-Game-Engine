/* 
 * File:   harbinger.h
 * Author: hammy
 *
 * Created on September 8, 2012, 3:29 PM
 */

#ifndef	__HGE_HARBINGER_H__
#define	__HGE_HARBINGER_H__

#ifndef NULL
	#define NULL 0x0
#endif

#include <string>
#include <cmath>
#include <queue>
#include <list>
#include <allegro5/allegro.h>
#include <glm/glm.hpp>
#include <SFML/System/Thread.hpp>
#include <SFML/Graphics.hpp>
#include "../../hamlibs/include/hamLibs.h"

namespace harbinger {
	class c_message;
	class c_messenger;
	class c_clock;
	class c_entity;
	class c_object;
	template <typename type> class c_resourceManager;
	template <int, int> class c_node;
	template <int, int> class c_node_gui;
} // end harbinger namespace

#include "events/message.h"

#include "utils/clock.h"
#include "utils/serialize.h"

#include "obj/resource.h"
#include "obj/object.h"

#include "scripting/script.h"

#endif	/* __HGE_HARBINGER_H__ */
