/* 
 * File:   bitmap.cpp
 * Author: hammy
 * 
 * Created on November 20, 2012, 9:09 PM
 */

#include <iostream>
#include <FreeImage.h>
#include "pipeline.h"
#include "bitmap.h"

//-----------------------------------------------------------------------------
//	Private Functions
//-----------------------------------------------------------------------------
void printImageLoadError( FREE_IMAGE_FORMAT fif, const char *msg );

FREE_IMAGE_FORMAT deduceImageFormat( const char* inFile );

int setBitmapFlags( FREE_IMAGE_FORMAT inFormat );

GLuint send2DToOpenGL( const void* buffer, int flags, int w, int h );

void send3DToOpenGL( unsigned index, const void* buffer, int w, int h );

//-----------------------------------------------------------------------------
//	Printing an error using FreeImage
//-----------------------------------------------------------------------------
void printImageLoadError( FREE_IMAGE_FORMAT fif, const char *msg ) {
	std::cerr
		<< "An error occurred when attempting to load an image:"
		<< "\n\tFormat: " << FreeImage_GetFormatFromFIF( fif )
		<< "\n\tOutput: " << msg
		<< std::endl;
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
GLuint send2DToOpenGL( const void* buffer, int flags, int w, int h ) {
	// Create the OpenGL texture
    GLuint texture( 0 );
	glGenTextures( 1, &texture );
    if ( !texture )
        return 0;
	glBindTexture( GL_TEXTURE_2D, texture );
	
	//Transfer the image data
	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_BGRA, GL_UNSIGNED_BYTE, buffer
	);
	
	//clamp each texture to the borders of whatever geometry holds it
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	
	// Create Mip Maps (if requested), then delete the FreeImage data,
	if ( flags & hge::c_bitmap::CREATE_MIPMAPS ) {
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glGenerateMipmap( GL_TEXTURE_2D );
	}
	
	printGlError( "Texture Loading" );
	glBindTexture( GL_TEXTURE_2D, 0 );
    return texture;
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
c_bitmap::c_bitmap( c_bitmap&& orig ) :
	c_resource(),
	oglTexture( orig.oglTexture ),
	bmpWidth( orig.bmpWidth ),
	bmpHeight( orig.bmpHeight )
{
	orig.oglTexture = 0;
	orig.bmpWidth = 0;
	orig.bmpHeight = 0;
}

c_bitmap& c_bitmap::operator =( c_bitmap&& bmpCopy ) {
	oglTexture = bmpCopy.oglTexture;
	bmpWidth = bmpCopy.bmpWidth;
	bmpHeight = bmpCopy.bmpHeight;
    
	bmpCopy.oglTexture = 0;
	bmpCopy.bmpWidth = 0;
	bmpCopy.bmpHeight = 0;
	
	return *this;
}

//-----------------------------------------------------------------------------
//	Bitmap - Check if an image is loaded within OpenGL
//-----------------------------------------------------------------------------
bool c_bitmap::isLoaded() const {
	return ( oglTexture != 0 );
}

//-----------------------------------------------------------------------------
//	Bitmap - Loading an image file
//-----------------------------------------------------------------------------
bool c_bitmap::load( cstr fileName, int imageFlags ) {
	FIBITMAP* image( HGE_NULL );
	FREE_IMAGE_FORMAT imageFormat( FIF_UNKNOWN );
	int FreeImageFlags( 0 );
	
	unload();
    if ( !fileName )
        return false;
	FreeImage_SetOutputMessage( printImageLoadError );
	
	imageFormat = deduceImageFormat( fileName );
	if ( imageFormat == FIF_UNKNOWN
	|| FreeImage_FIFSupportsReading( imageFormat ) == false ) {
		return false;
	}
	
	// Load the file
	FreeImageFlags = setBitmapFlags( imageFormat );
	image = FreeImage_Load( imageFormat, fileName, FreeImageFlags );
	
	if ( !image ) {
		return false;
	}
	
	// Convert the image to RGBA. Unload if it cannot be converted
	if ( FreeImage_GetColorType( image ) != FIC_RGBALPHA ) {
		FIBITMAP* temp = image;
		image = FreeImage_ConvertTo32Bits( temp );
		FreeImage_Unload( temp );
		if ( !image ) {
			return false;
		}
	}
	
	// All checks gave finished properly. Load the rest of the image
	bmpWidth    = FreeImage_GetWidth( image );
	bmpHeight   = FreeImage_GetHeight( image );
	oglTexture  = send2DToOpenGL(
        FreeImage_GetBits( image ), imageFlags, bmpWidth, bmpHeight
    );
	FreeImage_Unload( image );
    
	return oglTexture != 0;
}

//-----------------------------------------------------------------------------
//	Bitmap - Unloading
//-----------------------------------------------------------------------------
void c_bitmap::unload() {
	glDeleteTextures( 1, &oglTexture );
	oglTexture = 0; // insurance
	bmpWidth = 0;
	bmpHeight = 0;
}

//-----------------------------------------------------------------------------
//	Bitmap - Enable a texture within OpenGL
//-----------------------------------------------------------------------------
void c_bitmap::makeActive( int texUnit ) const {
	glActiveTexture( texUnit );
	glBindTexture( GL_TEXTURE_2D, oglTexture );
}

//-----------------------------------------------------------------------------
//	Bitmap - Disable an OpenGL texture
//-----------------------------------------------------------------------------
void c_bitmap::deActivate() const {
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, 0 );
}

/******************************************************************************
 * 3D (CUBEMAP) TEXTURES
******************************************************************************/
c_cubeMap::c_cubeMap( c_cubeMap&& orig ) :
    textureObj( orig.textureObj )
{
    orig.textureObj = 0;
}

c_cubeMap& c_cubeMap::operator =( c_cubeMap&& bmpCopy ) {
    textureObj = bmpCopy.textureObj;
    bmpCopy.textureObj = 0;
	return *this;
}

//-----------------------------------------------------------------------------
//	Main Cubemap loading function
//-----------------------------------------------------------------------------
bool c_cubeMap::loadTextures( const char* texFiles[ 6 ] ) {
    if ( textureObj != 0 )
        unload();
    
	FIBITMAP* image( HGE_NULL );
	FREE_IMAGE_FORMAT imageFormat( FIF_UNKNOWN );
	int FreeImageFlags( 0 );
	
	unload();
    
    glGenTextures( 1, &textureObj );
    if ( !textureObj ) {
        std::cerr
            << "ERROR: Unable to reserve a cube map texture on the GPU"
            << std::endl;
        return false;
    }
    glBindTexture( GL_TEXTURE_CUBE_MAP, textureObj );
    
	FreeImage_SetOutputMessage( printImageLoadError );
    
    for ( unsigned i = 0; i < 6; ++i ) {
        
        imageFormat = deduceImageFormat( texFiles[ i ] );
        
        if (    imageFormat == FIF_UNKNOWN
        ||      FreeImage_FIFSupportsReading( imageFormat ) == false
        ) {
            std::cerr
                << "ERROR: Unable to determine to file format of "
                << "the cubemap texture " << texFiles[ i ]
                << std::endl;
            unload();
            return false;
        }

        // Load the file
        FreeImageFlags = setBitmapFlags( imageFormat );
        image = FreeImage_Load( imageFormat, texFiles[ i ], FreeImageFlags );

        if ( !image ) {
            std::cerr
                << "ERROR: Unable to load the cubemap texture "
                << texFiles[ i ]
                << std::endl;
            unload();
            return false;
        }

        // Convert the image to RGBA. Unload if it cannot be converted
        if ( FreeImage_GetColorType( image ) != FIC_RGBALPHA ) {
            FIBITMAP* temp = image;
            image = FreeImage_ConvertTo32Bits( temp );
            FreeImage_Unload( temp );
            if ( !image ) {
                std::cerr
                    << "ERROR: Unable to convert the cubemap texture "
                    << texFiles[ i ] << " to RGBA format."
                    << std::endl;
                unload();
                return false;
            }
        }

        // All checks gave finished properly. Load the rest of the image
        send3DToOpenGL( i, FreeImage_GetBits( image ),
            FreeImage_GetWidth( image ), FreeImage_GetHeight( image )
        );
        
        FreeImage_Unload( image );
    }
    
    glBindTexture( GL_TEXTURE_CUBE_MAP, 0 );
    return true;
}

//-----------------------------------------------------------------------------
//	Cubemap unloading
//-----------------------------------------------------------------------------
void c_cubeMap::unload() {
    glDeleteTextures( 1, &textureObj );
    textureObj = 0;
}

//-----------------------------------------------------------------------------
//	Cubemap Binding
//-----------------------------------------------------------------------------
void c_cubeMap::activate( int texUnit ) const {
    glActiveTexture( texUnit );
    glBindTexture( GL_TEXTURE_CUBE_MAP, textureObj );
}

void c_cubeMap::deActivate() const {
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_CUBE_MAP, 0 );
}

} // end hge namespace
