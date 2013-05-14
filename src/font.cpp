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

using hamLibs::math::mat4;
using hamLibs::math::vec3;

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
    
    s_vertex* vertArray = new s_vertex[ HGE_MAX_NUM_GLYPHS * 4 ];
    
    for ( int i(0); i < HGE_MAX_NUM_GLYPHS; ++i ) {
        createCharBitmap( ftFace, i, vertArray );
    }
    
    int ret = sendVerticesToGPU( vertArray );
    if ( !ret )
        unload();
    
    scale = vec3( 1.f );
    c_drawableObj::update();
    
    FT_Done_Face( ftFace );
    FT_Done_FreeType( ftLib );
    delete [] vertArray;
    
    return ret;
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
    for ( int i = 0; i < HGE_MAX_NUM_GLYPHS; ++i )
        unloadCharTexture( charList[ i ] );
    
    glDeleteBuffers( 1, &vbo );
    glDeleteVertexArrays( 1, &vao );
    
//    glDeleteBuffers( 1, &ibo );
//    ibo = 0;
    
    vao = vbo = 0;
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

void c_font::createCharBitmap( FT_Face face, int index, s_vertex* vertArray ) {
    
    FT_Load_Glyph( face, FT_Get_Char_Index( face, index ), FT_LOAD_DEFAULT );
    
    FT_Render_Glyph( face->glyph, FT_RENDER_MODE_NORMAL );
    
    FT_Bitmap* ftBitmap = &face->glyph->bitmap;
    s_vertex* currVert  = vertArray + (index * 4);
    int& width          = charList[ index ].width;
    int& height         = charList[ index ].height;
    int& advX           = charList[ index ].advX;
    int& advY           = charList[ index ].advY;
    int& bearX          = charList[ index ].bearX;
    width               = nextPow2( ftBitmap->width );
    height              = nextPow2( ftBitmap->rows );
    GLubyte* data       = new GLubyte[ width*height ];
    
    // copy bitmap data into a format suitable for opengl
    for ( int h(0); h < height; ++h ) {
        for ( int w(0); w < width; ++w ) {
            data[h*width+w] = (h >= ftBitmap->rows || w >= ftBitmap->width)
            ? 0
            : ftBitmap->buffer[ (ftBitmap->rows-h-1) * ftBitmap->width+w ];
        }
    }
    
    //generate a texture for the current character
    sendTexturesToGPU( charList[ index ], data );
    
    // calculate the glyph data
    advX    = face->glyph->advance.x >> 6;
    bearX   = face->glyph->metrics.horiBearingX >> 6;
    advY    = (face->glyph->metrics.height - face->glyph->metrics.horiBearingY) >> 6;
    advY    = -advY;
    newLine = getMax(newLine, (int)face->glyph->metrics.height>>6);
        
    currVert[ 0 ].pos = vec3( 0.f,          float(advY+height), 0.f );
    currVert[ 1 ].pos = vec3( 0.f,          float(advY),        0.f );
    currVert[ 2 ].pos = vec3( float(width), float(advY+height), 0.f );
    currVert[ 3 ].pos = vec3( float(width), float(advY),        0.f );

    currVert[ 0 ].uv = vec2( 0.f, 1.f );
    currVert[ 1 ].uv = vec2( 0.f, 0.f );
    currVert[ 2 ].uv = vec2( 1.f, 1.f );
    currVert[ 3 ].uv = vec2( 1.f, 0.f );

    currVert[0].norm =
        currVert[1].norm =
            currVert[2].norm =
                currVert[3].norm = vec3( 0.f, 0.f, -1.f );

    currVert[0].tangent =
        currVert[1].tangent =
            currVert[2].tangent =
                currVert[3].tangent = vec3( 0.5f, 0.f, 0.5f );
    
    delete [] data;
}

///////////////////////////////////////////////////////////////////////////////
// FONT -- UPLOADING CHARACTER TEXTURES TO THE GPU
///////////////////////////////////////////////////////////////////////////////
void c_font::sendTexturesToGPU( c_font::charTexture& ct, const void* data ) {
    GLuint tex(0), sampler(0);
    glGenTextures( 1, &tex );
    glBindTexture( GL_TEXTURE_2D, tex );
    
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
        ct.width, ct.height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, data
    );
    
    glGenSamplers( 1, &sampler );
    glSamplerParameteri( sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glSamplerParameteri( sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glSamplerParameteri( sampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glSamplerParameteri( sampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    
    ct.textureId = tex;
    ct.samplerId = sampler;
}

///////////////////////////////////////////////////////////////////////////////
// FONT -- UPLOADING VERTEX DATA TO THE GPU
///////////////////////////////////////////////////////////////////////////////
bool c_font::sendVerticesToGPU( const s_vertex* vertArray ) {
	
	glGenVertexArrays( 1, &vao );
	glBindVertexArray( vao );
	glGenBuffers( 1, &vbo );
//	glGenBuffers( 1, &ibo );
    
    if ( !vao || !vbo ) {
        std::cerr
            << "An error occurred while loading font vertices"
            << std::endl;
        return false;
    }
	
	glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glBufferData(
        GL_ARRAY_BUFFER, sizeof( s_vertex ) * 4 * HGE_MAX_NUM_GLYPHS,
        vertArray, GL_STATIC_DRAW
    );
	printGlError( "Error while sending primitive data to the GPU.");
    
	glEnableVertexAttribArray( pipeline::VERTEX_ATTRIB );
	glVertexAttribPointer(
		pipeline::VERTEX_ATTRIB,
		ARRAY_SIZE_FROM_ELEMENTS( s_vertex::pos.v ), GL_FLOAT, GL_FALSE,
        sizeof( s_vertex ), (GLvoid*)offsetof( s_vertex, pos.v )
	);
    
	glEnableVertexAttribArray( pipeline::TEXTURE_ATTRIB );
	glVertexAttribPointer(
		pipeline::TEXTURE_ATTRIB,
		ARRAY_SIZE_FROM_ELEMENTS( s_vertex::uv.v ), GL_FLOAT, GL_FALSE,
        sizeof( s_vertex ), (GLvoid*)offsetof( s_vertex, uv.v )
	);
    
	glEnableVertexAttribArray( pipeline::NORMAL_ATTRIB );
	glVertexAttribPointer(
		pipeline::NORMAL_ATTRIB,
		ARRAY_SIZE_FROM_ELEMENTS( s_vertex::norm.v ), GL_FLOAT, GL_FALSE,
        sizeof( s_vertex ), (GLvoid*)offsetof( s_vertex, norm.v )
	);
    
	glEnableVertexAttribArray( pipeline::TANGENT_ATTRIB );
	glVertexAttribPointer(
		pipeline::TANGENT_ATTRIB,
		ARRAY_SIZE_FROM_ELEMENTS( s_vertex::tangent.v ), GL_FLOAT, GL_FALSE,
        sizeof( s_vertex ), (GLvoid*)offsetof( s_vertex, tangent.v )
	);
    
//    unsigned short indices[ 4 ] = { 0, 1, 2, 3 };
//	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibo );
//	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( indices ), indices, GL_STATIC_DRAW );
	
	glBindVertexArray( 0 );
    return true;
}

///////////////////////////////////////////////////////////////////////////////
// FONT -- UNLOADING CHAR TEXTURES
///////////////////////////////////////////////////////////////////////////////
void c_font::unloadCharTexture( c_font::charTexture& ct ) {
    glDeleteTextures( 1, &ct.textureId );
    glDeleteSamplers( 1, &ct.samplerId );
    ct.width = ct.height = ct.samplerId = ct.textureId = 0;
    ct.advX = ct.advY = ct.bearX = 0;
}

///////////////////////////////////////////////////////////////////////////////
// FONT -- DRAWING
///////////////////////////////////////////////////////////////////////////////
void c_font::draw() const {
    float xPos          = pos.v[0];
    float yPos          = pos.v[1];
    float fontScaleX    = scale.v[0];
    float fontScaleY    = scale.v[1];
    unsigned iter       = 0;
    int currChar        = 0;
    mat4&& rotMatrix    = quatToMat4( rot );
    
    glBindVertexArray   ( vao );
    glEnable            ( GL_BLEND );
    glBlendFunc         ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glDisable           ( GL_DEPTH_TEST );
    
    while ( text[ iter ] != '\0' ) {
        currChar = (int)text[ iter++ ];
        const charTexture& ct = charList[ currChar ];
        
        switch( currChar ) {
            case '\n':  case '\r':
                xPos = pos.v[0];
                yPos -= newLine*fontScaleY;
                break;
            case '\t':
                xPos += ((charList[' '].advX - charList[' '].bearX) * SPACES_PER_TAB ) * fontScaleX;
                break;
            default:
                glActiveTexture( GL_TEXTURE0 );
                glBindTexture( GL_TEXTURE_2D, ct.textureId );
                glBindSampler( 0, ct.samplerId );

                hge::pipeline::applyMatrix(
                    hge::pipeline::HGE_MODEL_MAT,
                    translate( modelMat, vec3( xPos, yPos, 0.f ) )
                    * rotMatrix
                );

                glDrawArrays( GL_TRIANGLE_STRIP, currChar*4, 4  );
//                glDrawElementsBaseVertex(
//                    GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, 0, currChar*4
//                );
                xPos += (ct.advX - ct.bearX) * fontScaleX;
        }
    }
    glDisable       ( GL_BLEND );
	glEnable        ( GL_DEPTH_TEST );
    glPolygonOffset ( 0.f, 0.f );
    glBindTexture   ( GL_TEXTURE_2D, 0 );
}

} // End Harbinger namespace
