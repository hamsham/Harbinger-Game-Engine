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

class HGE_API c_resource {
    public:
        c_resource		() {}
        c_resource      ( c_resource&& ) {}
        c_resource      ( const c_resource& ) {}
        ~c_resource		() {}
        
        c_resource&     operator =  ( c_resource&& ) = default;
        c_resource&     operator =  ( const c_resource& ) = default;

        virtual bool	isLoaded	() const = 0;
        virtual bool	load		( const char* filename, int flags ) = 0;
        virtual void	unload		() = 0;

        static bool		fileExists	( const char* filename );
        static unsigned fileSize	( const char* filename );
        static void		readFile	( const char* filename, char* buffer, unsigned length );
};

} // end hge namespace

#endif	/* __HGE_RESOURCE_H__ */
