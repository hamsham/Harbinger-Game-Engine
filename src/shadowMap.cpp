/* 
 * File:   shadowMap.cpp
 * Author: hammy
 * 
 * Created on April 18, 2013, 9:33 PM
 */

#include <iostream>
#include "shadowMap.h"

namespace hge {

/******************************************************************************
 * Shadow Map Initialization
******************************************************************************/
bool c_shadowMap::init( unsigned w, unsigned h ) {
    if ( fbo || shadow )
        terminate();
    
    std::cout << "Generating a shadow map: " << w << 'x' << h << std::endl;
    width = w;
    height = h;
    
    glGenFramebuffers( 1, &fbo );
    glGenTextures( 1, &shadow );
    printGlError( "Generating FBO and shadow map objects" );
    
    if ( !fbo || !shadow ) {
        terminate();
        std::cerr << "ERROR: Unable to create the shadow map." << std::endl;
        return false;
    }
    
    glBindTexture( GL_TEXTURE_2D, shadow );
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
        width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr
    );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
    
    glBindFramebuffer( GL_DRAW_FRAMEBUFFER, fbo );
    glFramebufferTexture2D(
        GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadow, 0
    );
    
    glDrawBuffer( GL_NONE );
    
    printGlError( "Setting shadowmap draw target" );
    GLenum fboStatus = glCheckFramebufferStatus( GL_FRAMEBUFFER );
    if ( fboStatus != GL_FRAMEBUFFER_COMPLETE ) {
        std::cerr << "Shadowmap FBO error. Status: 0x" << fboStatus << std::endl;
        terminate();
        return false;
    }
    
    std::cout << "Completed shadow map generation." << std::endl;
    return true;
}

/******************************************************************************
 * Shadow Map Termination
******************************************************************************/
void c_shadowMap::terminate() {
    glDeleteFramebuffers( 1, &fbo );
    glDeleteTextures( 1, &shadow );
    shadow = fbo = width = height = 0;
}

/******************************************************************************
 * Bind for reading
******************************************************************************/
void c_shadowMap::bindForReading( int texUnit ) {
    glActiveTexture( texUnit );
    glBindTexture( GL_TEXTURE_2D, shadow );
}

/******************************************************************************
 * Bind for writing
******************************************************************************/
void c_shadowMap::bindForWriting() {
    glBindFramebuffer( GL_DRAW_FRAMEBUFFER, fbo );
}

} // end hge namespace
