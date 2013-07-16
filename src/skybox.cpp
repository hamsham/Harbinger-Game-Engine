/* 
 * File:   skybox.cpp
 * Author: hammy
 * 
 * Created on April 25, 2013, 6:42 PM
 */

#include "skybox.h"

namespace hge {

bool skybox::load( const char* skyFiles[ 6 ] ) {
    if (    !skyTex.load( skyFiles[ 0 ], 0 )
    ||      !skyTex.load( skyFiles[ 1 ], 1 )
    ||      !skyTex.load( skyFiles[ 2 ], 2 )
    ||      !skyTex.load( skyFiles[ 3 ], 3 )
    ||      !skyTex.load( skyFiles[ 4 ], 4 )
    ||      !skyTex.load( skyFiles[ 5 ], 5 )
    ||      !spherePrim.createSphere( 10, 10 )
    ) {
        unload();
        return false;
    }
    return true;
}

void skybox::unload() {
    skyTex.unload();
    spherePrim.destroySphere();
}

void skybox::draw( const cubemap& c, const sphere& s ) {
    c.activate();
    s.draw();
    c.deActivate();
}

} // End hge namespace
