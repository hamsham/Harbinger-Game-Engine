/* 
 * File:   bitmap.cpp
 * Author: hammy
 * 
 * Created on November 20, 2012, 9:09 PM
 */

#include <iostream>
#include <utility>
#include <FreeImage.h>
#include "pipeline.h"
#include "bitmap.h"

//-----------------------------------------------------------------------------
//	Private Functions
//-----------------------------------------------------------------------------
void printImageLoadError( FREE_IMAGE_FORMAT fif, const char* msg );

bool loadImageFile( const char*, FIBITMAP**, int* w, int* h );

FREE_IMAGE_FORMAT deduceImageFormat( const char* inFile );

int setBitmapFlags( FREE_IMAGE_FORMAT inFormat );

void send2DToOpenGL( const void* buffer, int w, int h );

void send3DToOpenGL( unsigned index, const void* buffer, int w, int h );

//-----------------------------------------------------------------------------
//	Printing an error using FreeImage
//-----------------------------------------------------------------------------
void printImageLoadError( FREE_IMAGE_FORMAT fif, const char* msg ) {
	std::cerr
		<< "An error occurred when attempting to load an image:"
		<< "\n\tFormat: " << FreeImage_GetFormatFromFIF( fif )
		<< "\n\tOutput: " << msg
		<< std::endl;
}

//-----------------------------------------------------------------------------
//	Generic image loading function
//-----------------------------------------------------------------------------
bool loadImageFile( const char* filename, FIBITMAP** img, int* w, int* h ) {
    
	FREE_IMAGE_FORMAT imageFormat( FIF_UNKNOWN );
    
	int FreeImageFlags = 0;
    FIBITMAP* image = nullptr;
	
    if ( !filename ) {
        *w = *h = 0;
        return false;
    }
	FreeImage_SetOutputMessage( printImageLoadError );
	
	imageFormat = deduceImageFormat( filename );
	if ( imageFormat == FIF_UNKNOWN
	|| FreeImage_FIFSupportsReading( imageFormat ) == false ) {
		return false;
	}
	
	// Load the file
	FreeImageFlags = setBitmapFlags( imageFormat );
	image = FreeImage_Load( imageFormat, filename, FreeImageFlags );
	
	if ( !image ) {
        std::cerr
            << "ERROR: Could not load the image " << filename << std::endl;
        *w = *h = 0;
		return false;
	}
	
	// Convert the image to RGBA. Unload if it cannot be converted
	if ( FreeImage_GetColorType( image ) != FIC_RGBALPHA ) {
		FIBITMAP* temp = image;
		image = FreeImage_ConvertTo32Bits( temp );
		FreeImage_Unload( temp );
		if ( !image ) {
            std::cerr
                << "ERROR: Could not convert " << filename
                << " to RGBA format" << std::endl;
            *w = *h = 0;
			return false;
		}
	}
	
	// All checks gave finished properly. Load the rest of the image
	*w      = FreeImage_GetWidth( image );
	*h      = FreeImage_GetHeight( image );
    *img    = image;
    
    return true;
}

//-----------------------------------------------------------------------------
//	Deducing a file format
//-----------------------------------------------------------------------------
FREE_IMAGE_FORMAT deduceImageFormat( const char* inFile ) {
	FREE_IMAGE_FORMAT outFormat = FreeImage_GetFileType( inFile, 0 );
	if ( outFormat == FIF_UNKNOWN )
		outFormat = FreeImage_GetFIFFromFilename( inFile );
    return outFormat;
}

//-----------------------------------------------------------------------------
//	Setting flags that affect FreeImage Loading
//-----------------------------------------------------------------------------
int setBitmapFlags( FREE_IMAGE_FORMAT inFormat ) {
	switch( inFormat ) {
		case FIF_JPEG:  return JPEG_FAST | JPEG_EXIFROTATE;
		case FIF_TARGA: return TARGA_LOAD_RGB888;
		case FIF_ICO:   return ICO_MAKEALPHA;
		default:        return 0;
	}
}

//-----------------------------------------------------------------------------
//	Sending 2D Image Data to OpenGL
//-----------------------------------------------------------------------------
void send2DToOpenGL( const void* buffer, int w, int h ) {
	
	//Transfer the image data
	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_BGRA, GL_UNSIGNED_BYTE, buffer
	);
	
	//clamp each texture to the borders of whatever geometry holds it
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	
	// Create Mip Maps (if requested), then delete the FreeImage data,
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glGenerateMipmap( GL_TEXTURE_2D );
	
	printGlError( "Texture Loading" );
}

//-----------------------------------------------------------------------------
//	Sending 3D Image Data to OpenGL
//-----------------------------------------------------------------------------
void send3DToOpenGL( unsigned index, const void* buffer, int w, int h ) {
	
	//Transfer the image data
	glTexImage2D(
		GL_TEXTURE_CUBE_MAP_POSITIVE_X + index,
        0, GL_RGBA, w, h, 0, GL_BGRA, GL_UNSIGNED_BYTE, buffer
	);
	
	//clamp each texture to the borders of whatever geometry holds it
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );
	
	// Create Mip Maps (if requested), then delete the FreeImage data
    glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glGenerateMipmap( GL_TEXTURE_CUBE_MAP );
	
	printGlError( "Cubemap texture Loading" );
}

/******************************************************************************
 *      2D/3D Bitmaps
******************************************************************************/
namespace hge {
/******************************************************************************
 *      2D Bitmaps
******************************************************************************/
bitmap::bitmap( bitmap&& bmpCopy ) :
    resource    ( std::move( bmpCopy ) ),
    texture     ( std::move( bmpCopy ) ),
	bmpWidth    ( bmpCopy.bmpWidth ),
	bmpHeight   ( bmpCopy.bmpHeight )
{
	bmpCopy.bmpWidth = 0;
	bmpCopy.bmpHeight = 0;
}

bitmap& bitmap::operator =( bitmap&& bmpCopy ) {
    resource::operator=( std::move( bmpCopy ) );
    texture::operator=( std::move( bmpCopy ) );
	textureUnit = bmpCopy.textureUnit;
	textureId   = bmpCopy.textureId;
	bmpWidth    = bmpCopy.bmpWidth;
	bmpHeight   = bmpCopy.bmpHeight;
    
	bmpCopy.textureUnit = 0;
	bmpCopy.textureId   = 0;
	bmpCopy.bmpWidth    = 0;
	bmpCopy.bmpHeight   = 0;
	
	return *this;
}

//-----------------------------------------------------------------------------
//	Bitmap - Loading an image file
//-----------------------------------------------------------------------------
bool bitmap::load( const char* fileName, int ) {
    
    if ( textureId )
        unload();
    
	FIBITMAP* image = nullptr;
    
    if ( !loadImageFile( fileName, &image, &bmpWidth, &bmpHeight ) )
        return false;
    
	glGenTextures( 1, &textureId );
    
    if ( !textureId ) {
        std::cerr << "ERROR: OpenGL could not create a texture buffer" << std::endl;
        FreeImage_Unload( image );
        bmpWidth = bmpHeight = 0;
        return false;
    }
    
	glBindTexture( GL_TEXTURE_2D, textureId );
    send2DToOpenGL( FreeImage_GetBits( image ), bmpWidth, bmpHeight );
	glBindTexture( GL_TEXTURE_2D, 0 );
    
	FreeImage_Unload( image );
    
	return true;
}

//-----------------------------------------------------------------------------
//	Bitmap - Unloading
//-----------------------------------------------------------------------------
void bitmap::unload() {
    textureUnit = pipeline::HGE_TEXTURE_DEFAULT;
	glDeleteTextures( 1, &textureId );
	textureId = 0; // insurance
	bmpWidth = 0;
	bmpHeight = 0;
}

/******************************************************************************
 * 3D (CUBEMAP) TEXTURES
******************************************************************************/
cubemap::cubemap( cubemap&& bmpCopy ) :
    resource    ( std::move( bmpCopy ) ),
    texture     ( std::move( bmpCopy ) )
{}

cubemap& cubemap::operator =( cubemap&& bmpCopy ) {
    resource::operator=( std::move( bmpCopy ) );
    texture::operator=( std::move( bmpCopy ) );
	textureUnit = bmpCopy.textureUnit;
	textureId   = bmpCopy.textureId;
    
	bmpCopy.textureUnit = 0;
	bmpCopy.textureId   = 0;
    
	return *this;
}

//-----------------------------------------------------------------------------
//	Main Cubemap loading function
//-----------------------------------------------------------------------------
bool cubemap::load( const char* fileName, int cubeIndex ) {
    
    int texWidth = 0;
    int texHeight = 0;
	FIBITMAP* image = nullptr;
    
    if ( !loadImageFile( fileName, &image, &texWidth, &texHeight ) )
        return false;
    
    if ( !textureId ) {
        glGenTextures( 1, &textureId );
        
        if ( !textureId ) {
            std::cerr
                << "ERROR: OpenGL could not create a texture buffer"
                << std::endl;
            FreeImage_Unload( image );
            unload();
            return false;
        }
    }
    
    // All checks gave finished properly. Load the rest of the image
    glBindTexture( GL_TEXTURE_CUBE_MAP, textureId );
    send3DToOpenGL( cubeIndex, FreeImage_GetBits( image ), texWidth, texHeight );
    glBindTexture( GL_TEXTURE_CUBE_MAP, 0 );

    FreeImage_Unload( image );
    
    return true;
}

//-----------------------------------------------------------------------------
//	Cubemap unloading
//-----------------------------------------------------------------------------
void cubemap::unload() {
    textureUnit = pipeline::HGE_TEXTURE_DEFAULT;
    glDeleteTextures( 1, &textureId );
    textureId = 0;
}

} // end hge namespace
