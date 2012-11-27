/* 
 * File:   types.h
 * Author: hammy
 *
 * Created on October 25, 2012, 4:51 PM
 */

#ifndef __HGE_TYPES_H__
#define	__HGE_TYPES_H__

//This header file includes forward declarations of all data types used by the engine

#ifdef NULL
	#undef NULL
#endif
#ifndef NULL
	#define NULL nullptr
#endif /* NULL Redefinition */

#include "scripting/script.h"

namespace harbinger {
	
	//------------------------------------------------------------------------
	//	DATA TYPES
	//------------------------------------------------------------------------
	
	// Messaging Header
	template <typename message_type> class s_message;
	template <typename message_type> class c_messenger;
	struct s_logMessage; // message type
	struct s_scriptMessage; // message type
	class c_logHandler;
	class c_resourceHandler;
	
	// Resource Header
	class c_resource;
	class c_scriptManager;
	
	// Utils Header
	class c_clock;
	
	//Bitmap Header
	class c_bitmap;
	
	// Geometric Header
	template <unsigned int numVerts> struct s_polygon;
	struct s_vertex2d;
	struct s_vertex3d;
	struct s_vertex4d;
	
	// Misc Header
	template <typename T1, typename T2> struct s_pair;
	
	//------------------------------------------------------------------------
	//	TYPEDEF's
	//------------------------------------------------------------------------
	//P.O.D.
	typedef unsigned char		uchar;
	typedef unsigned int		uint;
	typedef unsigned short		ushort;
	typedef unsigned long int	ulong;
	
	typedef hamLibs::size_t		size_t;
	
	//function pointers
	typedef void (*voidFunc_t)();
	typedef void (*voidFuncArray_t [])();
	
	//3D Geometry
	typedef s_polygon< 2 > s_line;
	typedef s_polygon< 3 > s_tri;
	typedef s_polygon< 4 > s_quad;
	
	//Scripting Containers & Structures
	typedef std::map< ulong, c_script* >		scriptMap_t;
	typedef std::map< ulong, c_scriptVarBase* >	scriptVarMap_t;
	typedef std::map< ulong, c_scriptFuncBase* >	scriptFuncMap_t;
	typedef std::list< c_script* >			scriptList_t;
	typedef scriptList_t::size_type			scriptListSize_t;
	
	typedef s_message< c_scriptFuncBase& >		s_scriptFuncMessage_t;
	typedef c_messenger< c_scriptFuncBase& >	c_scriptFuncMessenger_t;
	
} // end harbinger namespace

#endif	/* __HGE_TYPES_H__ */
