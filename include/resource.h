/* 
 * File:   resource.h
 * Author: hammy
 *
 * Created on August 13, 2012, 9:13 PM
 */

#ifndef __HGE_RESOURCE_H__
#define	__HGE_RESOURCE_H__

#include "types.h"

namespace hge {

class HGE_API resource {
    public:
        resource		() {}
        resource        ( resource&& ) {}
        resource        ( const resource& ) {}
        ~resource		() {}
        
        resource&       operator =  ( resource&& ) = default;
        resource&       operator =  ( const resource& ) = default;

        virtual bool	isLoaded	() const = 0;
        virtual bool	load		( const char* filename, int flags ) = 0;
        virtual void	unload		() = 0;

        static bool		fileExists	( const char* filename );
        static unsigned fileSize	( const char* filename );
        static void		readFile	( const char* filename, char* buffer, unsigned length );
};

} // end hge namespace

#endif	/* __HGE_RESOURCE_H__ */
