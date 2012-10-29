/* 
 * File:   types.h
 * Author: hammy
 *
 * Created on October 25, 2012, 4:51 PM
 */

#ifndef __HGE_TYPES__
#define	__HGE_TYPES__

//This header file includes forward declarations of all data types used by the engine

#include "scripting/script.h"

namespace harbinger {
	
	//------------------------------------------------------------------------
	//	NAMESPACES
	//------------------------------------------------------------------------
	namespace serialization {}
	
	//------------------------------------------------------------------------
	//	DATA TYPES
	//------------------------------------------------------------------------
	class c_message;
	class c_messenger;
	class c_clock;
	class c_entity;
	class c_object;
	template < typename type> class c_resourceManager;
	class c_scriptManager;
	
	//------------------------------------------------------------------------
	//	TYPEDEF's
	//------------------------------------------------------------------------
	typedef std::list< c_script* > scriptList;
	typedef std::vector< c_script* > scriptArray;
	
} // end harbinger namespace

#endif	/* __HGE_TYPES__ */

