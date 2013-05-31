/* 
 * File:   font.cpp
 * Author: hammy
 * 
 * Created on April 1, 2013, 11:04 PM
 */

#include <iostream>
#include <stdexcept>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "font.h"

#include "pipeline.h"

using namespace hamLibs::math;

struct font_vertex {
	vec3 pos;
	vec2 uv;
	vec3 norm;
};

void printFontError( const char* msg, int error ) {
    std::cerr << msg;
    if ( error ) std::cerr << error;
    std::cerr << std::endl;
}

namespace hge {

///////////////////////////////////////////////////////////////////////////////
// FONT -- LOADING
///////////////////////////////////////////////////////////////////////////////
bool c_font::load( const char* filename, int fontsize ) {
    FT_Library ftLib;
    FT_Face ftFace;
    FT_Error ftError;
    
    // Initialize FreeType
    ftError = FT_Init_FreeType( &ftLib );
    if ( ftError ) {
        printFontError( "An error occurred while initializing FreeType: ", ftError );
        return false;
    }
    // Load the font file
    ftError = FT_New_Face( ftLib, filename, 0, &ftFace );
    if ( ftError ) {
        printFontError( "An error occurred while loading a font file: ", ftError );
        printFontError( filename, 0 );
        FT_Done_FreeType( ftLib );
        return false;
    }
    // Modify the font's size
    FT_Set_Pixel_Sizes( ftFace, fontsize, fontsize );
    
    loadGlyphs( ftFace );
    
    FT_Done_Face( ftFace );
    FT_Done_FreeType( ftLib );
    
    return true;
}

///////////////////////////////////////////////////////////////////////////////
// FONT -- CHECKING IF LOADED
///////////////////////////////////////////////////////////////////////////////
bool c_font::isLoaded() const {
    return textureId != 0;
}

///////////////////////////////////////////////////////////////////////////////
// FONT -- UNLOADING
///////////////////////////////////////////////////////////////////////////////
void c_font::unload() {
    glDeleteTextures( 1, &textureId );
    
    textureId = 0;
    
    maxWidth = maxHeight = 0;
}

///////////////////////////////////////////////////////////////////////////////
// FONT -- GENERATING CHAR BITMAPS
///////////////////////////////////////////////////////////////////////////////
inline int nextPow2( int x ) {
    if (x < 0)
        return 1;
    --x;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    ++x;
    return x;
}

inline int getMax( int x, int y ) {
    return x>y ? x:y;
}

void c_font::loadGlyphs( FT_Face face ) {
    int maxCharWidth = 0;
    int maxCharHeight = 0;
    GLubyte* dataArray[ MAX_NUM_GLYPHS ];
    
    for ( int index = 0; index < MAX_NUM_GLYPHS; ++index ) {
        FT_Load_Glyph( face, FT_Get_Char_Index( face, index ), FT_LOAD_DEFAULT );

        FT_Render_Glyph( face->glyph, FT_RENDER_MODE_NORMAL );

        FT_Bitmap* ftBitmap = &face->glyph->bitmap;
        int& width          = metrics[ index ].width;
        int& height         = metrics[ index ].height;
        int& advX           = metrics[ index ].advX;
        int& advY           = metrics[ index ].advY;
        int& bearX          = metrics[ index ].bearX;
        width               = nextPow2( ftBitmap->width );
        height              = nextPow2( ftBitmap->rows );
        
        GLubyte* data       = dataArray[ index ] = new GLubyte[ width*height ];

        // copy bitmap data into a format suitable for opengl
        for ( int h(0); h < height; ++h ) {
            for ( int w(0); w < width; ++w ) {
                data[h*width+w] = (h >= ftBitmap->rows || w >= ftBitmap->width)
                ? 0
                : ftBitmap->buffer[ (ftBitmap->rows-h-1) * ftBitmap->width+w ];
            }
        }
        
        maxWidth = getMax( maxCharWidth, width );
        maxHeight = getMax( maxCharHeight, height );
        
        // calculate the glyph data
        advX    = face->glyph->advance.x >> 6;
        bearX   = face->glyph->metrics.horiBearingX >> 6;
        advY    = (face->glyph->metrics.height - face->glyph->metrics.horiBearingY) >> 6;
        advY    = -advY;
        newLine = getMax(newLine, (int)face->glyph->metrics.height>>6);
    }
    
    //generate a texture atlas
    createCharAtlas( dataArray );
}

///////////////////////////////////////////////////////////////////////////////
// FONT -- UPLOADING CHARACTER TEXTURES TO THE GPU
///////////////////////////////////////////////////////////////////////////////
void c_font::createCharAtlas( GLubyte* bitmaps[ MAX_NUM_GLYPHS ] ) {
    
    const int texDimension = HL_ROUND( std::sqrt( (float)MAX_NUM_GLYPHS ) );
    
    glGenTextures( 1, &textureId );
    glBindTexture( GL_TEXTURE_2D, textureId );
    
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
        (texDimension * maxWidth), (texDimension * maxHeight),
        0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, nullptr
    );
    
    int iter = 0;
    for ( int i = 0; i < texDimension; ++i ) {
        for ( int j = 0; j < texDimension; ++j ) {
            glTexSubImage2D(
                GL_TEXTURE_2D, 0, (i*maxWidth), (j*maxHeight),
                metrics[ iter ].width, metrics[ iter ].height,
                GL_RED, GL_UNSIGNED_BYTE, bitmaps[ iter ]
            );
            printGlError( "Error while updating a font atlas" );
            
            vec2 pos = vec2( i*maxWidth, j*maxHeight );
            vec2 uv = vec2(
                pos[0] + metrics[ iter ].width, pos[1] + metrics[ iter ].height
            );
            
            metrics[ iter ].pos[0]  = (pos[0]) / (texDimension*maxWidth);
            metrics[ iter ].pos[1]  = (pos[1]) / (texDimension*maxHeight);
            metrics[ iter ].uv[0]   = (uv[0]) / (texDimension*maxWidth);
            metrics[ iter ].uv[1]   = (uv[1]-1.f) / (texDimension*maxHeight);
            
            delete [] bitmaps[ iter ];
            ++iter;
        }
    }
    
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	
	// Create Mip Maps (if requested), then delete the FreeImage data,
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glGenerateMipmap( GL_TEXTURE_2D );
    printGlError( "Error while creating mipmaps for a font atlas" );
}

///////////////////////////////////////////////////////////////////////////////
//	FONT - TEXTURE ACTIVATION/BINDING
///////////////////////////////////////////////////////////////////////////////
void c_font::activate() const {
	glActiveTexture ( textureUnit );
	glBindTexture   ( GL_TEXTURE_2D, textureId );
}

///////////////////////////////////////////////////////////////////////////////
//	FONT - TEXTURE DEACTIVATION
///////////////////////////////////////////////////////////////////////////////
void c_font::deActivate() const {
	glActiveTexture ( textureUnit );
	glBindTexture   ( GL_TEXTURE_2D, 0 );
}

///////////////////////////////////////////////////////////////////////////////
// TEST STRING CLASS
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// STRING - UNLOADING
///////////////////////////////////////////////////////////////////////////////
void c_string::clearString() {
    glDeleteVertexArrays( 1, &vao );
    glDeleteBuffers( 1, &vbo );
    
    indices.clear();
    
    vao = vbo = 0;
    numChars = 0;
}

///////////////////////////////////////////////////////////////////////////////
// STRING - Buffer Data
///////////////////////////////////////////////////////////////////////////////
void c_string::createVertexBuffer( unsigned numVerts ) {
    if ( !vao )
        glGenVertexArrays( 1, &vao );
    
	glBindVertexArray( vao );
    
    if ( !vbo )
        glGenBuffers( 1, &vbo );
	
	glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glBufferData(
        GL_ARRAY_BUFFER, sizeof( font_vertex ) * 4 * numVerts,
        nullptr, GL_DYNAMIC_DRAW
    );
	printGlError( "Error while creating a string object's vertex buffer.");
    
	glEnableVertexAttribArray( pipeline::VERTEX_ATTRIB );
	glVertexAttribPointer(
		pipeline::VERTEX_ATTRIB,
		ARRAY_SIZE_FROM_ELEMENTS( font_vertex::pos.v ), GL_FLOAT, GL_FALSE,
        sizeof( font_vertex ), (GLvoid*)offsetof( font_vertex, pos.v )
	);
    
	glEnableVertexAttribArray( pipeline::TEXTURE_ATTRIB );
	glVertexAttribPointer(
		pipeline::TEXTURE_ATTRIB,
		ARRAY_SIZE_FROM_ELEMENTS( font_vertex::uv.v ), GL_FLOAT, GL_FALSE,
        sizeof( font_vertex ), (GLvoid*)offsetof( font_vertex, uv.v )
	);
    
	glEnableVertexAttribArray( pipeline::NORMAL_ATTRIB );
	glVertexAttribPointer(
		pipeline::NORMAL_ATTRIB,
		ARRAY_SIZE_FROM_ELEMENTS( font_vertex::norm.v ), GL_FLOAT, GL_FALSE,
        sizeof( font_vertex ), (GLvoid*)offsetof( font_vertex, norm.v )
	);
    
	glDisableVertexAttribArray( pipeline::TANGENT_ATTRIB );
    
	printGlError( "Error while resizing a string buffer on the GPU.");
}

///////////////////////////////////////////////////////////////////////////////
// STRING - TEXT FORMATTING
///////////////////////////////////////////////////////////////////////////////
void c_string::setString( const c_font& font, const char* str ) {
    
    if ( !str ) {
        clearString();
        return;
    }
    
    int xPos = 0;
    int yPos = 0;
    font_vertex tempQuad[ 4 ];
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
        indices.reserve( numChars * 2 );
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
                tempQuad[3].norm = vec3( 0.f, 0.f, -1.f );
    
    // Create and send the vertices to OpenGL
    int charCount = 0;
    int indexIter = 0;
    for ( int i = 0; str[ i ] != '\0'; ++i ) {
        const int currChar = (int)str[ i ];
        const c_font::charMetrics& m = font.metrics[ currChar ];
        
        if ( currChar == '\n' ) {
            yPos -= font.newLine;
            xPos = 0.f;
        }
        else if ( currChar == '\r' ) {
            xPos = 0.f;
        }
        else if ( currChar == ' ' ) {
            xPos += (font.metrics[' '].advX - font.metrics[' '].bearX);
        }
        else if ( currChar == '\t' ) {
            xPos += (font.metrics[' '].advX - font.metrics[' '].bearX)
                * c_font::SPACES_PER_TAB;
        }
        else {
            // populate the index array
            indices[ indexIter ] = indexIter * 4;
            indices[ indexIter + numChars ] = 4;
            ++indexIter;
            
            /*
             * 0--------2
             * |     /  |
             * |   /    |
             * | /      |
             * 1--------3
             */
            tempQuad[ 0 ].pos = vec3( xPos,           yPos+m.advY+m.height,   0.f );
            tempQuad[ 1 ].pos = vec3( xPos,           yPos+m.advY,            0.f );
            tempQuad[ 2 ].pos = vec3( xPos+m.width,   yPos+m.advY+m.height,   0.f );
            tempQuad[ 3 ].pos = vec3( xPos+m.width,   yPos+m.advY,            0.f );

            tempQuad[ 0 ].uv = vec2( m.pos[0],       m.uv[1] );
            tempQuad[ 1 ].uv = vec2( m.pos[0],       m.pos[1] );
            tempQuad[ 2 ].uv = vec2( m.uv[0],        m.uv[1] );
            tempQuad[ 3 ].uv = vec2( m.uv[0],        m.pos[1] );

            xPos += m.advX - m.bearX;

            glBufferSubData(
                GL_ARRAY_BUFFER, sizeof( tempQuad ) * charCount++,
                sizeof( tempQuad ), tempQuad
            );
            printGlError( "Error while updating string buffer data on the GPU.");
        }
    }
	glBindVertexArray( 0 );
}

void c_string::draw() const {
    glEnable            ( GL_BLEND );
    glBlendFunc         ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glDepthMask         ( GL_FALSE );
    glBindVertexArray   ( vao );

    glMultiDrawArrays(
        GL_TRIANGLE_STRIP,
        (const GLint*)indices.data(),
        (const GLsizei*)(indices.data() + numChars),
        numChars
    );

    glBindVertexArray   ( 0 );
    glDepthMask         ( GL_TRUE );
    glDisable           ( GL_BLEND );
}

} // End Harbinger namespace
