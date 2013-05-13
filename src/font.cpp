/* 
 * File:   font.cpp
 * Author: hammy
 * 
 * Created on April 1, 2013, 11:04 PM
 */

#include <iostream>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "font.h"
#include "pipeline.h"

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
    
    // Prepare OpenGL for the font data
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );
    glGenBuffers( 1, &vbo );
    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    
    vboArray vboData;
    for ( int i(0); i < 256; ++i ) {
        createCharBitmap( ftFace, i, vboData );
    }
    
    // send the data to OpenGL
    glBufferData( GL_ARRAY_BUFFER, vboData.size(), &vboData[0], GL_STATIC_DRAW );
    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, sizeof( vec2 )*2, 0 );
    glEnableVertexAttribArray( 1 );
    glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, sizeof( vec2 )*2, (void*)(sizeof(vec2)) );
    lineHeight = fontsize;
    scale = vec3( fontsize );
    
    FT_Done_Face( ftFace );
    FT_Done_FreeType( ftLib );
    
    return true;
}

///////////////////////////////////////////////////////////////////////////////
// FONT -- CHECKING IF LOADED
///////////////////////////////////////////////////////////////////////////////
bool c_font::isLoaded() const {
    return vao != 0;
}

///////////////////////////////////////////////////////////////////////////////
// FONT -- UNLOADING
///////////////////////////////////////////////////////////////////////////////
void c_font::unload() {
    for ( int i = 0; i < 256; ++i )
        unloadCharTexture( charList[ i ] );
    
    glDeleteBuffers( 1, &vbo );
    glDeleteVertexArrays( 1, &vao );
    
    vao = vbo = lineHeight = 0;
    pos = scale = vec3( 0.f );
    text = HGE_NULL;
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

void c_font::createCharBitmap( FT_Face face, int index, vboArray& vboData ) {
    
    FT_Load_Glyph( face, FT_Get_Char_Index( face, index ), FT_LOAD_DEFAULT );
    
    FT_Render_Glyph( face->glyph, FT_RENDER_MODE_NORMAL );
    FT_Bitmap* ftBitmap = &face->glyph->bitmap;
    
    // create an array for the character bitmap data
    int width       = nextPow2( ftBitmap->width );
    int height      = nextPow2( ftBitmap->rows );
    GLubyte* data   = new GLubyte[ width*height ];
    
    // copy bitmap data into a format suitable for opengl
    for ( int h(0); h < height; ++h ) {
        for ( int w(0); w < width; ++w ) {
            data[h*width+w] = (h >= ftBitmap->rows || w >= ftBitmap->width)
            ? 0
            : ftBitmap->buffer[ (ftBitmap->rows-h-1) * ftBitmap->width+w ];
        }
    }
    
    //generate a texture for the current character
    generateCharTexture( charList[ index ], width, height, data );
    
    // calculate the glyph data
    charList[ index ].advX = face->glyph->advance.x >> 6;
    charList[ index ].bearX = face->glyph->metrics.horiBearingX >> 6;
    int advY =
        (face->glyph->metrics.height - face->glyph->metrics.horiBearingY) >> 6;
    newLine = getMax(newLine, (int)face->glyph->metrics.height>>6);
    
    vec2 coords[] = {
        vec2( 0.f, float(-advY+height) ),
        vec2( 0.f, float(-advY) ),
        vec2( float(width), float(-advY+height) ),
        vec2( float(width), float(-advY) )
    };
    vec2 texQuad[] = {
        vec2( 0.f, 1.f ),
        vec2( 0.f, 0.f ),
        vec2( 1.f, 1.f ),
        vec2( 1.f, 0.f )
    };
    
    for ( int i = 0; i < 4; ++i ) {
        vboData.insert(
            vboData.end(),
            (unsigned char*)&coords[i],
            (unsigned char*)&coords[i] + sizeof( vec2 )
        );
        vboData.insert(
            vboData.end(),
            (unsigned char*)&texQuad[i],
            (unsigned char*)&texQuad[i] + sizeof( vec2 )
        );
    }
    
    delete [] data;
}

///////////////////////////////////////////////////////////////////////////////
// FONT -- GENERATING CHAR STRUCTURE TEXTURES
///////////////////////////////////////////////////////////////////////////////
void c_font::generateCharTexture(
    c_font::charTexture& ct,
    int w,
    int h,
    const void* data
) {
    GLuint tex(0), sampler(0);
    glGenTextures( 1, &tex );
    glBindTexture( GL_TEXTURE_2D, tex );
    
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
        w, h, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, data
    );
    
    glGenSamplers( 1, &sampler );
    glSamplerParameteri( sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glSamplerParameteri( sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glSamplerParameteri( sampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glSamplerParameteri( sampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    
    ct.width = w;
    ct.height = h;
    ct.textureId = tex;
    ct.samplerId = sampler;
}

///////////////////////////////////////////////////////////////////////////////
// FONT -- UNLOADING CHAR TEXTURES
///////////////////////////////////////////////////////////////////////////////
void c_font::unloadCharTexture( c_font::charTexture& ct ) {
    glDeleteTextures( 1, &ct.textureId );
    glDeleteSamplers( 1, &ct.samplerId );
    ct.width = ct.height = ct.samplerId = ct.textureId = 0;
    ct.advX = ct.bearX = 0;
}

///////////////////////////////////////////////////////////////////////////////
// FONT -- DRAWING TEST
///////////////////////////////////////////////////////////////////////////////
void c_font::draw() const {
    if ( !isLoaded() )
        return;
    
    float xPos = pos.v[0];
    float yPos = pos.v[1];
    float fontScaleX = scale.v[0]/lineHeight;
    float fontScaleY = scale.v[1]/lineHeight;
    unsigned iter = 0;
    int currChar = 0;
    
    mat4 rotMatrix( quatToMat4( rot ) );
    
    glBindVertexArray( vao );
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glDisable(GL_DEPTH_TEST);
    
    while ( text[ iter ] != '\0' ) {
        currChar = (int)text[ iter++ ];
        const charTexture& ct = charList[ currChar ];
        
        if ( currChar == '\n' || currChar == '\r' ) {
            xPos = pos.v[0];
            yPos -= newLine*fontScaleY;
        }
        
        else if ( currChar == '\t' ) {
            xPos += ((charList[' '].advX - charList[' '].bearX) * SPACES_PER_TAB ) * fontScaleX;
        }
        
        else {
            glActiveTexture( GL_TEXTURE0 );
            glBindTexture( GL_TEXTURE_2D, ct.textureId );
            glBindSampler( 0, ct.samplerId );
            
            hge::pipeline::applyMatrix(
                hge::pipeline::HGE_MODEL_MAT,
                translate( modelMat, vec3( xPos, yPos, 0.f ) )
                * rotMatrix
            );

            glDrawArrays( GL_TRIANGLE_STRIP, currChar*4, 4  );
            xPos += (ct.advX - ct.bearX) * fontScaleX;
        }
    }
    glDisable( GL_BLEND );
	glEnable(GL_DEPTH_TEST);
    glBindTexture( GL_TEXTURE_2D, 0 );
}

} // End Harbinger namespace
