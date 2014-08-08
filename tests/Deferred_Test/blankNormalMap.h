/* 
 * File:   blankNormalMap.h
 * Author: hammy
 *
 * Created on August 16, 2013, 1:54 AM
 */

#ifndef __BLANK_NORMAL_MAP_H__
#define	__BLANK_NORMAL_MAP_H__

#include "main.h"

class blankNormalMap {
    private:
        unsigned texId = 0;
        
    public:
        blankNormalMap() {}
        ~blankNormalMap() { terminate(); }
        
        bool init();
        void terminate();
        
        void activate() const;
        void deActivate() const;
};

inline void blankNormalMap::terminate() {
    glDeleteTextures( 1, &texId );
    texId = 0;
}

inline void blankNormalMap::activate() const {
    glActiveTexture( hge::pipeline::HGE_TEXTURE_NORMAL );
    glBindTexture( GL_TEXTURE_2D, texId );
}

inline void blankNormalMap::deActivate() const {
    glActiveTexture( hge::pipeline::HGE_TEXTURE_NORMAL );
    glBindTexture( GL_TEXTURE_2D, 0 );
}

#endif	/* __BLANK_NORMAL_MAP_H__ */

