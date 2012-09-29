/* 
 * File:   serialize.h
 * Author: hammy
 *
 * Created on September 9, 2012, 2:20 PM
 */

#ifndef	__HGE_SERIALIZE_H__
#define	__HGE_SERIALIZE_H__
namespace harbinger {
	namespace serialize {
	
		bool saveToFile( const c_entity& entity, ... );
		
	} // end serialize namespace
} // end harbinger namespace

#endif	/* __HGE_SERIALIZE_H__ */
