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
    clearString();
    
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
    
    resetDrawMode();
}

///////////////////////////////////////////////////////////////////////////////
// STRING - Buffer Data
// This function will bind the current object's VAO and VBO
///////////////////////////////////////////////////////////////////////////////
void text::createVertexBuffer( const char* str ) {
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
    
    if ( !vao ) {
        glGenVertexArrays( 1, &vao );
    }
    glBindVertexArray( vao );
    
    if ( !vbo ) {
        glGenBuffers( 1, &vbo );
    }
	glBindBuffer( GL_ARRAY_BUFFER, vbo );
    pipeline::enablePlainVertexAttribs();
    
    // Check if the text buffer needs to be resized
    if ( numChars == (indices.size() / 2)) {
        return;
    }

    try {
        indices.resize( numChars * 2 );
        glBufferData(
            GL_ARRAY_BUFFER, sizeof( plainVertex ) * 4 * numChars, nullptr, GL_DYNAMIC_DRAW
        );
    }
    catch( const std::length_error& err ) {
        std::cerr << "ERROR: Unable to create the string of text: " << str << std::endl;
        clearString();
        return;
    }
	printGlError( "Error while creating a string object's vertex buffer.");
}

///////////////////////////////////////////////////////////////////////////////
// STRING - TEXT FORMATTING
// This function will un-bind the VAO and VBO after uploading the text data
///////////////////////////////////////////////////////////////////////////////
void text::setString( const font& f, const char* str ) {
    
    if ( !str ) {
        createVertexBuffer( 0 );
        return;
    }
    
    float xPos = 0.f;
    float yPos = 0.f;
    plainVertex* tempQuad = nullptr;
    
    createVertexBuffer( str );
    
//    tempQuad = (plainVertex*)glMapBuffer( GL_ARRAY_BUFFER, GL_WRITE_ONLY );
    // Harbinger is not currently multi-threaded. This operation will perform much quicker.
    tempQuad = (plainVertex*)glMapBufferRange(
        GL_ARRAY_BUFFER, 0, sizeof( plainVertex ) * 4 * numChars,
        GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT
    );
    
    if ( !tempQuad ) {
        glBindVertexArray( 0 );
        return;
    }
    
    // Create and send the vertices to OpenGL
    int charCount = 0;
    for ( int i = 0; str[ i ] != '\0'; ++i ) {
        const int currChar = (int)str[ i ];
        
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
            const font::metric_t& m = f.metrics[ currChar ];
            
            // populate the index array
            indices[ charCount ] = charCount * 4;
            indices[ charCount + numChars ] = 4;
            
            // 0--------2
            // |     /  |
            // |   /    |
            // | /      |
            // 1--------3
            //
            tempQuad[ 0 ].pos = vec3( xPos,         yPos+m.advY+m.height,   0.f );
            tempQuad[ 1 ].pos = vec3( xPos,         yPos+m.advY,            0.f );
            tempQuad[ 2 ].pos = vec3( xPos+m.width, yPos+m.advY+m.height,   0.f );
            tempQuad[ 3 ].pos = vec3( xPos+m.width, yPos+m.advY,            0.f );
            
            tempQuad[ 0 ].uv = vec2( m.pos[0],      m.uv[1] );
            tempQuad[ 1 ].uv = vec2( m.pos[0],      m.pos[1] );
            tempQuad[ 2 ].uv = vec2( m.uv[0],       m.uv[1] );
            tempQuad[ 3 ].uv = vec2( m.uv[0],       m.pos[1] );
            
            tempQuad[0].norm =
                tempQuad[1].norm =
                    tempQuad[2].norm =
                        tempQuad[3].norm = vec3( 0.f, 0.f, 1.f );
            
            xPos += m.advX - m.bearX;
            
            tempQuad += 4; // increment the pointer to the buffer map
            ++charCount;
            printGlError( "Error while updating string buffer data on the GPU.");
        }
    }
    
    if ( !glUnmapBuffer( GL_ARRAY_BUFFER ) ) {
        std::cerr << "ERROR: Unable to unmap the current text buffer." << std::endl;
    }
	glBindVertexArray( 0 );
}

///////////////////////////////////////////////////////////////////////////////
// STRING - ATTRIBUTE MANIPULATION
///////////////////////////////////////////////////////////////////////////////
void text::enableAttribute( pipeline::attribute a ) {
    HL_DEBUG_ASSERT(
        (a > pipeline::HGE_ATTRIB_INVALID) &&
        (a <= pipeline::HGE_ATTRIB_NORMAL)
    );
    
    glBindVertexArray( vao );
    glEnableVertexAttribArray( (GLint)a );
    glBindVertexArray( 0 );
}

void text::disableAttribute( pipeline::attribute a ) {
    HL_DEBUG_ASSERT(
        (a > pipeline::HGE_ATTRIB_INVALID) &&
        (a <= pipeline::HGE_ATTRIB_NORMAL)
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
    glBindVertexArray( vao );

    glMultiDrawArrays(
        renderMode,
        (const GLint*)indices.data(),
        (const GLsizei*)(indices.data() + numChars),
        numChars
    );

    glBindVertexArray( 0 );
    glDisable( GL_BLEND );
}

} // End Harbinger namespace
