/* 
 * File:   billboard.cpp
 * Author: hammy
 * 
 * Created on May 20, 2013, 12:14 AM
 */

#include <iostream>
#include <utility>
#include "billboard.h"

namespace hge {

/******************************************************************************
 * move semantics
******************************************************************************/
billboard::billboard( billboard&& b ) :
    drawable( std::move( b ) ),
    vbo( b.vbo ),
    numBmps( b.numBmps)
{
    b.vbo = 0;
    b.numBmps = 0;
}

billboard& billboard::operator = ( billboard&& b ) {
    clearImages();
    
    drawable::operator = ( std::move( b ) );
    
    vbo = b.vbo;
    b.vbo = 0;
    
    numBmps = b.numBmps;
    b.numBmps = 0;
}

/******************************************************************************
 * setting the number of images to draw
******************************************************************************/
bool billboard::setNumImages( unsigned r, unsigned c ) {
    
    if ( !vao ) {
        glGenVertexArrays( 1, &vao );
        if ( !vao ) {
            std::cerr
                << "ERROR: Unable to create a VAO for a billboard object."
                << std::endl;
            return false;
        }
        
        glGenBuffers( 1, &vbo );
        if ( !vbo ) {
            std::cerr
                << "ERROR: Unable to create a VBO for a billboard object."
                << std::endl;
            glDeleteVertexArrays( 1, &vao );
            vao = 0;
            return false;
        }
    }
    
    numBmps = r * c;
    
    vec3* positions = new vec3[ numBmps ];
    float* sizes = new float[ numBmps ];
    
    for ( unsigned x = 0; x < r; ++x ) {
        for ( unsigned y = 0; y < c; ++y ) {
            positions[ x * c + y ] = vec3( (float)x, 0.f, (float)y );
            sizes[ x * c + y ] = 1.f;
        }
    }
    
    glBindVertexArray( vao );
    glEnableVertexAttribArray( 0 );
    glEnableVertexAttribArray( 1 );
    
    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glBufferData( GL_ARRAY_BUFFER, (sizeof(vec3)+sizeof(float))*numBmps,  nullptr, GL_STREAM_DRAW );
    
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(vec3)*numBmps, positions );
    
    glVertexAttribPointer( 1, 1, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)(sizeof(vec3)*numBmps) );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(vec3)*numBmps, sizeof(float)*numBmps, sizes );
    
    glBindVertexArray( 0 );
    
    delete [] positions;
    delete [] sizes;
    
    return true;
}

/******************************************************************************
 * clearing all drawable points from the buffer
******************************************************************************/
void billboard::clearImages() {
    glDeleteVertexArrays( 1, &vao );
    glDeleteBuffers( 1, &vbo );
    
    vao = vbo = 0;
}

/******************************************************************************
 * Billboard positioning
******************************************************************************/
void billboard::setImagePos( unsigned index, const vec3& pos ) {
    if ( index >= numBmps ) return;
    
    glBindVertexArray( vao );
    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof( vec3 ) * index, sizeof( vec3 ), pos.v );
    
    glBindVertexArray( 0 );
    printGlError("Billboard positioning error");
}

/******************************************************************************
 * Billboard Scaling
******************************************************************************/
void billboard::setImageSize( unsigned index, float size ) {
    if ( index >= numBmps ) return;
    
    glBindVertexArray( vao );
    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glBufferSubData(
        GL_ARRAY_BUFFER,
        (sizeof( vec3 ) * numBmps) + (sizeof( float ) * index),
        sizeof( float ), &size
    );
    
    glBindVertexArray( 0 );
    printGlError("Billboard scaling error");
}

/******************************************************************************
 * Billboard positioning
******************************************************************************/
void billboard::setImagePositions( const vec3* pos ) {
    if ( !pos ) return;
    
    glBindVertexArray( vao );
    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(vec3)*numBmps, pos );
    
    glBindVertexArray( 0 );
    printGlError("Billboard positioning error");
}

/******************************************************************************
 * Billboard Scaling
******************************************************************************/
void billboard::setImageSizes( float* sizes ) {
    if ( !sizes ) return;
    
    glBindVertexArray( vao );
    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(vec3)*numBmps, sizeof(float)*numBmps, sizes );
    
    glBindVertexArray( 0 );
    printGlError("Billboard scaling error");
}

} // end hge namespace
