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

class HGE_API c_skyBox {
    private:
        hge::c_cubeMap  cubeMap;
        hge::c_sphere   spherePrim;
        
    public:
        c_skyBox    () {}
        ~c_skyBox   () {}
        
        c_skyBox    ( const c_skyBox& ) = delete;
        c_skyBox    ( c_skyBox&& ) = default;
        c_skyBox&   operator =  ( const c_skyBox& ) = delete;
        c_skyBox&   operator =  ( c_skyBox&& ) = default;
        bool        operator == ( const c_skyBox& s ) { return (cubeMap == s.cubeMap) && (spherePrim == s.spherePrim); }
        bool        operator != ( const c_skyBox& s ) { return (cubeMap != s.cubeMap) || (spherePrim != s.spherePrim); }
        
        bool load   ( const char* skyFiles[ 6 ] );
        void unload ();
        
        void draw   () const;
        static void draw ( const c_cubeMap&, const c_sphere& );
};


} // End hge namespace

#endif	/* __HGE_SKYBOX_H__ */
