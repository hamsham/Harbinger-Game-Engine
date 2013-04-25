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

class HGE_API c_shadowMap {
    private:
        unsigned width = 0;
        unsigned height = 0;
        unsigned fbo = 0;
        unsigned shadow = 0;
        
    public:
        c_shadowMap() {}
        ~c_shadowMap() { terminate(); }
        
        bool init( unsigned w = 256, unsigned h = 256 );
        void terminate();
        
        void bindForWriting();
        void bindForReading( int texUnit = hge::pipeline::HGE_TEXTURE_SHADOWMAP );
};

} // end hge namespace

#endif	/* __HGE_SHADOWMAP_H__ */
