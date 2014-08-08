/* 
 * File:   blankNormalMap.cpp
 * Author: hammy
 * 
 * Created on August 16, 2013, 1:54 AM
 */

#include "blankNormalMap.h"

bool blankNormalMap::init() {
    if ( !texId ) {
        glGenTextures( 1, &texId );
        
        if ( !texId )
            return false;
    }
    
    const unsigned char mapRGB[] = {
        255, 128, 128
    };
    
    glBindTexture( GL_TEXTURE_2D, texId );
    
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_BGR, GL_UNSIGNED_BYTE, mapRGB );
    
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    
    glBindTexture( GL_TEXTURE_2D, 0 );
    
    printGlError( "Generating a blank normal map." );
    
    return true;
}
