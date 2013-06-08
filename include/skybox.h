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
 * Right, left, bottom, top, back, front.
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
        bool        operator == ( const skybox& s ) { return (skyTex == s.skyTex) && (spherePrim == s.spherePrim); }
        bool        operator != ( const skybox& s ) { return (skyTex != s.skyTex) || (spherePrim != s.spherePrim); }
        
        bool load   ( const char* skyFiles[ 6 ] );
        void unload ();
        
        void draw   () const;
        
        static void draw ( const cubemap&, const sphere& );
};


} // End hge namespace

#endif	/* __HGE_SKYBOX_H__ */
