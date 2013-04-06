/* 
 * File:   bitmap.cpp
 * Author: hammy
 * 
 * Created on November 20, 2012, 9:09 PM
 */

#include <iostream>
#include "pipeline.h"
#include "bitmap.h"

namespace hge {

namespace {
//-----------------------------------------------------------------------------
//	Private Functions
//-----------------------------------------------------------------------------
void printImageLoadError( FREE_IMAGE_FORMAT fif, const char *msg );

FREE_IMAGE_FORMAT deduceImageFormat( cstr inFile );

int setBitmapFlags( FREE_IMAGE_FORMAT inFormat );

GLuint sendToOpenGL( const void* buffer, int flags, int w, int h );

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
FREE_IMAGE_FORMAT deduceImageFormat( cstr inFile ) {
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
//	Sending Image Data to OpenGL
//-----------------------------------------------------------------------------
GLuint sendToOpenGL( const void* buffer, int flags, int w, int h ) {
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
	if ( flags & c_bitmap::CREATE_MIPMAPS ) {
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glGenerateMipmap( GL_TEXTURE_2D );
	}
	
	printGLError( "Texture Loading" );
	glBindTexture( GL_TEXTURE_2D, 0 );
    return texture;
}

} // end anonymous namespace

//-----------------------------------------------------------------------------
//	Bitmap - Con/Destruction
//-----------------------------------------------------------------------------
c_bitmap::c_bitmap() :
	c_resource(),
	oglTexture( 0 ),
	bmpWidth( 0 ),
	bmpHeight( 0 )
{}

c_bitmap::c_bitmap( const c_bitmap& orig ) :
	c_resource(),
	oglTexture( orig.oglTexture ),
	bmpWidth( orig.bmpWidth ),
	bmpHeight( orig.bmpHeight )
{}

c_bitmap::~c_bitmap() {
	unload();	
}

c_bitmap& c_bitmap::operator =( const c_bitmap& bmpCopy ) {
	oglTexture = bmpCopy.oglTexture;
	bmpWidth = bmpCopy.bmpWidth;
	bmpHeight = bmpCopy.bmpHeight;
	
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
	oglTexture  = sendToOpenGL(
        FreeImage_GetBits( image ), imageFlags, bmpWidth, bmpHeight
    );
	FreeImage_Unload( image );
    
	return true;
}

//-----------------------------------------------------------------------------
//	Bitmap - Unloading
//-----------------------------------------------------------------------------
void c_bitmap::unload() {
	if ( oglTexture != 0 )
		glDeleteTextures( 1, &oglTexture );
	oglTexture = 0; // insurance
	bmpWidth = 0;
	bmpHeight = 0;
}

//-----------------------------------------------------------------------------
//	Bitmap - Enable a texture within OpenGL
//-----------------------------------------------------------------------------
void c_bitmap::makeActive( GLuint texUnit ) const {
	//glActiveTexture( texUnit );
	glBindTexture( GL_TEXTURE_2D, oglTexture );
}

//-----------------------------------------------------------------------------
//	Bitmap - Disable an OpenGL texture
//-----------------------------------------------------------------------------
void c_bitmap::deActivate() const {
	//glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, 0 );
}

} // end hge namespace
