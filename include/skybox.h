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

namespace hge {

class HGE_API c_skyBox : virtual public hge::c_drawableObj {
    private:
        hge::c_cubeMap  cubeMap;
        hge::c_sphere   spherePrim;
        
    public:
        c_skyBox    () {}
        ~c_skyBox   () {}
        
        bool load   ( const char* skyFiles[ 6 ] );
        void unload ();
        
        void draw   () const;
};

} // End hge namespace

#endif	/* __HGE_SKYBOX_H__ */
