/* 
 * File:   resource.h
 * Author: hammy
 *
 * Created on August 13, 2012, 9:13 PM
 */

#ifndef __HGE_RESOURCE_H__
#define	__HGE_RESOURCE_H__

#include "global.h"

class c_resource {
	public:
		c_resource		() {}
		~c_resource		() {}
		
		virtual bool	isLoaded	() const = 0;
		virtual bool	load		( cstr filename, int flags ) = 0;
		virtual void	unload		() = 0;

		static bool		fileExists	( cstr filename );
		static uint		fileSize	( cstr filename );
		static void		readFile	( cstr filename, char* buffer, uint length );
};

#endif	/* __HGE_RESOURCE_H__ */

