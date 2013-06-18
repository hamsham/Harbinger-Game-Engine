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

void skybox::draw() const {
    glCullFace( GL_FRONT );
    glDepthFunc( GL_LEQUAL );
    
    skyTex.activate();
    
    spherePrim.draw();
    
    skyTex.deActivate();
    
    glCullFace( GL_BACK );
    glDepthFunc( GL_LESS );
}

void skybox::draw( const cubemap& c, const sphere& s ) {
    glCullFace( GL_FRONT );
    glDepthRange( 1, 1 );
    glDepthFunc( GL_LEQUAL );
    
    c.activate();
    
    s.draw();
    
    c.deActivate();
    
    glCullFace( GL_BACK );
    glDepthRange( 0, 1 );
    glDepthFunc( GL_LESS );
}

} // End hge namespace
