/* 
 * File:   text.cpp
 * Author: hammy
 * 
 * Created on August 3, 2013, 7:15 PM
 */

#include <iostream>
#include <stdexcept>
#include <utility>

#include "font.h"
#include "text.h"
#include "geometry.h"

namespace hge {

///////////////////////////////////////////////////////////////////////////////
// TEST STRING CLASS
///////////////////////////////////////////////////////////////////////////////
text::text( text&& s ) :
    drawable( std::move( s ) ),
    vbo( s.vbo ),
    numChars( s.numChars ),
    indices( std::move( s.indices ) )
{}

text& text::operator = ( text&& s ) {
    drawable::operator = ( std::move( s ) );
    
    vbo = s.vbo;
    s.vbo = 0;
    
    numChars = s.numChars;
    s.numChars = 0;
    
    indices = std::move( s.indices );
}

///////////////////////////////////////////////////////////////////////////////
// STRING - UNLOADING
///////////////////////////////////////////////////////////////////////////////
void text::clearString() {
    glDeleteVertexArrays( 1, &vao );
    glDeleteBuffers( 1, &vbo );
    
    indices.clear();
    
    vao = vbo = 0;
    numChars = 0;
}

///////////////////////////////////////////////////////////////////////////////
// STRING - Buffer Data
///////////////////////////////////////////////////////////////////////////////
void text::createVertexBuffer( unsigned numVerts ) {
    if ( !vao )
        glGenVertexArrays( 1, &vao );
    
	glBindVertexArray( vao );
    
    if ( !vbo )
        glGenBuffers( 1, &vbo );
	
	glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glBufferData(
        GL_ARRAY_BUFFER, sizeof( plainVertex ) * 4 * numVerts,
        nullptr, GL_DYNAMIC_DRAW
    );
	printGlError( "Error while creating a string object's vertex buffer.");
    
    pipeline::enablePlainVertexAttribs();
    resetDrawMode();
}

///////////////////////////////////////////////////////////////////////////////
// STRING - TEXT FORMATTING
///////////////////////////////////////////////////////////////////////////////
void text::setString( const font& f, const char* str ) {
    
    if ( !str ) {
        clearString();
        return;
    }
    
    float xPos = 0.f;
    float yPos = 0.f;
    plainVertex tempQuad[ 4 ];
    numChars = 0;
    
    //count all the whitespace
    for( int i = 0; str[ i ] != '\0'; ++i ) {
        if (    str[ i ] != '\n'
        &&      str[ i ] != '\r'
        &&      str[ i ] != '\t'
        &&      str[ i ] != ' '
        ) {
            ++numChars;
        }
    }
	
    // Resize the text buffer
    try {
        indices.resize( numChars * 2 );
    }
    catch( const std::length_error& err ) {
        std::cerr << "ERROR: Unable to create the string of text: " << str << std::endl;
        clearString();
        return;
    }
    createVertexBuffer( numChars );
    
    tempQuad[0].norm =
        tempQuad[1].norm =
            tempQuad[2].norm =
                tempQuad[3].norm = vec3( 0.f, 0.f, 1.f );
    
    // Create and send the vertices to OpenGL
    int charCount = 0;
    for ( int i = 0; str[ i ] != '\0'; ++i ) {
        const int currChar = (int)str[ i ];
        const font::metric_t& m = f.metrics[ currChar ];
        
        if ( currChar == '\n' ) {
            yPos -= f.newLine;
            xPos = 0.f;
        }
        else if ( currChar == '\r' ) {
            xPos = 0.f;
        }
        else if ( currChar == ' ' ) {
            xPos += (f.metrics[' '].advX - f.metrics[' '].bearX);
        }
        else if ( currChar == '\t' ) {
            xPos += (f.metrics[' '].advX - f.metrics[' '].bearX)
                * font::SPACES_PER_TAB;
        }
        else {
            // populate the index array
            indices[ charCount ] = charCount * 4;
            indices[ charCount + numChars ] = 4;
            
            /*
             * 0--------2
             * |     /  |
             * |   /    |
             * | /      |
             * 1--------3
             */
            tempQuad[ 0 ].pos = vec3( xPos,         yPos+m.advY+m.height,   0.f );
            tempQuad[ 1 ].pos = vec3( xPos,         yPos+m.advY,            0.f );
            tempQuad[ 2 ].pos = vec3( xPos+m.width, yPos+m.advY+m.height,   0.f );
            tempQuad[ 3 ].pos = vec3( xPos+m.width, yPos+m.advY,            0.f );
            
            tempQuad[ 0 ].uv = vec2( m.pos[0],      m.uv[1] );
            tempQuad[ 1 ].uv = vec2( m.pos[0],      m.pos[1] );
            tempQuad[ 2 ].uv = vec2( m.uv[0],       m.uv[1] );
            tempQuad[ 3 ].uv = vec2( m.uv[0],       m.pos[1] );
            
            xPos += m.advX - m.bearX;

            glBufferSubData(
                GL_ARRAY_BUFFER, sizeof( tempQuad ) * charCount,
                sizeof( tempQuad ), tempQuad
            );
            ++charCount;
            printGlError( "Error while updating string buffer data on the GPU.");
        }
    }
	glBindVertexArray( 0 );
}

///////////////////////////////////////////////////////////////////////////////
// STRING - ATTRIBUTE MANIPULATION
///////////////////////////////////////////////////////////////////////////////
void text::enableAttribute( pipeline::attribute a ) {
    HL_DEBUG_ASSERT(
        (a > pipeline::INVALID_UNIFORM) &&
        (a <= pipeline::NORMAL_ATTRIB)
    );
    
    glBindVertexArray( vao );
    glEnableVertexAttribArray( (GLint)a );
    glBindVertexArray( 0 );
}

void text::disableAttribute( pipeline::attribute a ) {
    HL_DEBUG_ASSERT(
        (a > pipeline::INVALID_UNIFORM) &&
        (a <= pipeline::NORMAL_ATTRIB)
    );
    glBindVertexArray( vao );
    glDisableVertexAttribArray( (GLint)a );
    glBindVertexArray( 0 );
}

///////////////////////////////////////////////////////////////////////////////
// STRING - DRAWING
///////////////////////////////////////////////////////////////////////////////
void text::draw() const {
    glEnable( GL_BLEND );
    
    // Premultiplied alpha
//    glBlendEquationSeparate( GL_FUNC_ADD, GL_FUNC_ADD );
//    glBlendFuncSeparate( GL_ONE, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO );
    
    // Additive Blending
    glBlendFunc( GL_ONE, GL_ONE );
    glDepthMask( GL_FALSE );
    glBindVertexArray( vao );

    glMultiDrawArrays(
        renderMode,
        (const GLint*)indices.data(),
        (const GLsizei*)(indices.data() + numChars),
        numChars
    );

    glBindVertexArray( 0 );
    glDepthMask( GL_TRUE );
    glDisable( GL_BLEND );
}

} // End Harbinger namespace
