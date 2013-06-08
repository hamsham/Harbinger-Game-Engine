/* 
 * File:   shadowMap.h
 * Author: hammy
 *
 * Created on April 18, 2013, 9:33 PM
 */

#ifndef __HGE_SHADOWMAP_H__
#define	__HGE_SHADOWMAP_H__

#include "pipeline.h"

namespace hge {

class HGE_API shadowMap {
    private:
        unsigned width  = 0;
        unsigned height = 0;
        unsigned fbo    = 0;
        unsigned shadow = 0;
        
    public:
        
        enum : unsigned { DEFAULT_SHADOW_SIZE = 256 };
        
        shadowMap()             {}
        ~shadowMap()            { terminate(); }
        
        bool    init            ( unsigned w = DEFAULT_SHADOW_SIZE, unsigned h = DEFAULT_SHADOW_SIZE );
        void    terminate       ();
        
        void    bindForWriting  ();
        void    bindForReading  ( int texUnit = hge::pipeline::HGE_TEXTURE_SHADOWMAP );
};

} // end hge namespace

#endif	/* __HGE_SHADOWMAP_H__ */
