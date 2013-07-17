/* 
 * File:   skybox.h
 * Author: hammy
 *
 * Created on April 25, 2013, 6:42 PM
 * 
 * Note:
 *      This is just for convenience, although it is still useful enough to
 *      include in Harbinger.
 */

#ifndef __HGE_SKYBOX_H__
#define	__HGE_SKYBOX_H__

#include "bitmap.h"
#include "primitives.h"

/*
 * Skybox images should be loaded in the following order:
 * Right, left, bottom, top, front, back.
 * 
 * Use the graph below as a guide when importing the textures:
 *              --------
 *             |        |
 *             |  top   |
 *             |        |
 *              --------
 *  --------    --------    --------    --------
 * |        |  |        |  |        |  |        |
 * |  left  |  |  front |  |  right |  |  back  |
 * |        |  |        |  |        |  |        |
 *  --------    --------    --------    --------
 *              --------
 *             |        |
 *             | bottom |
 *             |        |
 *              --------
 */

namespace hge {

class HGE_API skybox {
    private:
        hge::cubemap  skyTex;
        hge::sphere   spherePrim;
        
    public:
        skybox      () {}
        ~skybox     () {}
        
        skybox      ( const skybox& ) = delete;
        skybox      ( skybox&& ) = default;
        skybox&     operator =  ( const skybox& ) = delete;
        skybox&     operator =  ( skybox&& ) = default;
        
        bool load( const char* skyFiles[ 6 ] );
        
        void unload() {
            skyTex.unload();
            spherePrim.destroySphere();
        }
        
        inline static void draw( const cubemap& c, const sphere& s ) {
            c.activate();
            s.draw();
            c.deActivate();
        }
        
        inline void draw() const { draw( skyTex, spherePrim ); }
};

} // End hge namespace

#endif	/* __HGE_SKYBOX_H__ */
