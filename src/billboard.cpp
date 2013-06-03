/* 
 * File:   billboard.cpp
 * Author: hammy
 * 
 * Created on May 20, 2013, 12:14 AM
 */

#include <iostream>

#include "billboard.h"

namespace hge {

bool c_billboard::setNumInstances( unsigned r, unsigned c ) {
    
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
    
    glBindVertexArray( vao );
    glEnableVertexAttribArray( pipeline::VERTEX_ATTRIB );
    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );
    
    numBmps = r * c;
    
    vec3* positions = new vec3[ r * c ];
    
    for ( unsigned x = 0; x < r; ++x ) {
        for ( unsigned y = 0; y < c; ++y ) {
            positions[ x * c + y ] = vec3( (float)x, 0.f, (float)y );
        }
    }
    
    glBufferData(
        GL_ARRAY_BUFFER, sizeof( vec3 ) * numBmps, positions,
        GL_STREAM_DRAW
    );
    
    delete [] positions;
    
    return true;
}

void c_billboard::clearInstances() {
    glDeleteVertexArrays( 1, &vao );
    glDeleteBuffers( 1, &vbo );
    
    vao = vbo = 0;
}

void c_billboard::setInstancePos( unsigned index, const vec3& pos ) {
    if ( index > numBmps ) return;
    
    glBindVertexArray( vao );
    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    
    glBufferSubData(
        GL_ARRAY_BUFFER,
        sizeof( vec3 ) * index,
        sizeof( vec3 ), pos.v
    );
    
    glBindVertexArray( 0 );
}

void c_billboard::draw() const {
    glBindVertexArray( vao );
    glDrawArrays( GL_POINTS, 0, numBmps );
    glBindVertexArray( 0 );
}

} // end hge namespace
