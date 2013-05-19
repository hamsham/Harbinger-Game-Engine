/* 
 * File:   skybox.cpp
 * Author: hammy
 * 
 * Created on April 25, 2013, 6:42 PM
 */

#include "skybox.h"

namespace hge {

bool c_skyBox::load( const char* skyFiles[ 6 ] ) {
    if (    !cubeMap.load( skyFiles[ 0 ], 0 )
    ||      !cubeMap.load( skyFiles[ 1 ], 1 )
    ||      !cubeMap.load( skyFiles[ 2 ], 2 )
    ||      !cubeMap.load( skyFiles[ 3 ], 3 )
    ||      !cubeMap.load( skyFiles[ 4 ], 4 )
    ||      !cubeMap.load( skyFiles[ 5 ], 5 )
    ||      !spherePrim.createSphere( 10, 10 )
    ) {
        unload();
        return false;
    }
    return true;
}

void c_skyBox::unload() {
    cubeMap.unload();
    spherePrim.destroySphere();
}

void c_skyBox::draw() const {
    glCullFace( GL_FRONT );
    glDepthFunc( GL_LEQUAL );
    
    cubeMap.activate();
    
    spherePrim.draw();
    
    cubeMap.deActivate();
    
    glCullFace( GL_BACK );
    glDepthFunc( GL_LESS );
}

} // End hge namespace
