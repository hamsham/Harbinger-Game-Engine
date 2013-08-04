/* 
 * File:   font.cpp
 * Author: hammy
 * 
 * Created on April 1, 2013, 11:04 PM
 */

#include <iostream>
#include <utility>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "font.h"

using namespace hamLibs::math;

void printFontError( const char* msg, int error ) {
    std::cerr << msg;
    if ( error ) std::cerr << error;
    std::cerr << std::endl;
}

namespace hge {

///////////////////////////////////////////////////////////////////////////////
// FONT -- Move semantics
///////////////////////////////////////////////////////////////////////////////
font::font( font&& f ) :
    texture( std::move( f ) ),
    newLine( f.newLine ),
    maxWidth( f.maxWidth ),
    maxHeight( f.maxHeight ),
    metrics( f.metrics )
{}

font& font::operator = ( font&& f ) {
    texture::operator = ( std::move( f ) );
    
    newLine = f.newLine;
    f.newLine = 0;
    
    maxWidth = f.maxWidth;
    f.maxWidth = 0;
    
    maxHeight = f.maxHeight;
    f.maxHeight = 0;
    
    metrics = f.metrics;
    f.metrics = nullptr;
}

///////////////////////////////////////////////////////////////////////////////
// FONT -- LOADING
///////////////////////////////////////////////////////////////////////////////
bool font::load( const char* filename, int fontsize ) {
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
    
    metrics = new metric_t[ MAX_NUM_GLYPHS ];
    loadGlyphs( ftFace );
    
    FT_Done_Face( ftFace );
    FT_Done_FreeType( ftLib );
    
    return true;
}

///////////////////////////////////////////////////////////////////////////////
// FONT -- CHECKING IF LOADED
///////////////////////////////////////////////////////////////////////////////
bool font::isLoaded() const {
    return textureId != 0;
}

///////////////////////////////////////////////////////////////////////////////
// FONT -- UNLOADING
///////////////////////////////////////////////////////////////////////////////
void font::unload() {
    textureUnit = pipeline::HGE_TEXTURE_DEFAULT;
    
    glDeleteTextures( 1, &textureId );
    textureId = 0;
    
    maxWidth = maxHeight = 0;
    
    delete [] metrics;
    metrics = nullptr;
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

void font::loadGlyphs( FT_Face face ) {
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
        
        // Allocate memory for a glyph. Memory is deleted in "createCharAtlas()"
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
void font::createCharAtlas( GLubyte* bitmaps[ MAX_NUM_GLYPHS ] ) {
    
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
            
            // Scale the Position coords so that they clamp within the atlas' size
            metrics[ iter ].pos[0]  = (pos[0]) / (texDimension*maxWidth);
            metrics[ iter ].pos[1]  = (pos[1]) / (texDimension*maxHeight);
            // scale the UV components so they are merely offsets of the positions
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

} // end hge namespace
