/* 
 * File:   types.h
 * Author: hammy
 *
 * Created on October 25, 2012, 4:51 PM
 */

#ifndef __HGE_TYPES__
#define	__HGE_TYPES__

//This header file includes forward declarations of all data types used by the engine

#ifdef NULL
	#undef NULL
#endif
#ifndef NULL
	#define NULL nullptr
#endif /* NULL Redefinition */

#define HGE_INLINE HL_INLINE //redefinition of the HamLibs macro

#include "scripting/script.h"

namespace harbinger {
	
	//------------------------------------------------------------------------
	//	NAMESPACES
	//------------------------------------------------------------------------
	namespace serialization {}
	
	//------------------------------------------------------------------------
	//	DATA TYPES
	//------------------------------------------------------------------------
	template <typename type> class s_message;
	template <typename type> class c_messenger;
	template <typename type> class c_resourceManager;
	class c_clock;
	class c_entity;
	class c_object;
	class c_scriptManager;
	
	template <unsigned int numVerts> struct s_polygon;
	struct s_vertex2d;
	struct s_vertex3d;
	struct s_vertex4d;
	
	//misc types
	template < typename T1, typename T2 > struct s_pair;
	
	//------------------------------------------------------------------------
	//	TYPEDEF's
	//------------------------------------------------------------------------
	//P.O.D.
	typedef unsigned int		uint;
	typedef unsigned long int	ulong;
	
	//Geometry
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

#endif	/* __HGE_TYPES__ */

