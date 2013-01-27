/* 
 * File:   bitmap.cpp
 * Author: hammy
 * 
 * Created on November 20, 2012, 9:09 PM
 */

#include <iostream>
#include "pipelineGL.h"
#include "bitmap.h"
using namespace std;

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
//	Bitmap - Misc Private Functions
//-----------------------------------------------------------------------------
void printImageLoadError(FREE_IMAGE_FORMAT fif, const char *msg) {
	std::cerr
		<< "An error occurred when attempting to load an image:"
		<< "\n\tFormat: " << FreeImage_GetFormatFromFIF( fif )
		<< "\n\tOutput: " << msg
		<< std::endl;
}

void c_bitmap::deduceImageFormat( cstr inFile, FREE_IMAGE_FORMAT& inFormat ) const {
	if (inFile == 0)
		inFormat = FIF_UNKNOWN;
	
	inFormat = FreeImage_GetFileType( inFile, 0 );
	if ( inFormat == FIF_UNKNOWN )
		inFormat = FreeImage_GetFIFFromFilename( inFile );
}

void c_bitmap::setBitmapFlags( FREE_IMAGE_FORMAT inFormat, int& outFlags ) {
	switch( inFormat ) {
		case FIF_JPEG:
			outFlags = JPEG_FAST | JPEG_EXIFROTATE;
			break;
		case FIF_TARGA:
			outFlags = TARGA_LOAD_RGB888;
			break;
		case FIF_ICO:
			outFlags = ICO_MAKEALPHA;
			break;
		default:
			outFlags = 0;
	}
}

//-----------------------------------------------------------------------------
//	Bitmap - Memory-Based Operations
//-----------------------------------------------------------------------------
bool c_bitmap::isLoaded() const {
	return ( oglTexture != 0 );
}

bool c_bitmap::load( cstr fileName, int imageFlags ) {
	FIBITMAP* image( HGE_NULL );
	FREE_IMAGE_FORMAT imageFormat( FIF_UNKNOWN );
	int FreeImageFlags( 0 );
	
	unload();
	FreeImage_SetOutputMessage( printImageLoadError );
	
	deduceImageFormat( fileName, imageFormat );
	if ( imageFormat == FIF_UNKNOWN
	|| FreeImage_FIFSupportsReading( imageFormat ) == false ) {
		return false;
	}
	
	// Load the file
	setBitmapFlags( imageFormat, FreeImageFlags );
	image = FreeImage_Load( imageFormat, fileName, FreeImageFlags );
	
	if (!image) {
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
	bmpWidth = FreeImage_GetWidth( image );
	bmpHeight = FreeImage_GetHeight( image );
	
	// Create the OpenGL texture
	glGenTextures( 1, &oglTexture );
	glBindTexture( GL_TEXTURE_2D, oglTexture );
	
	//create the image
	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGBA, bmpWidth, bmpHeight, 0,
		GL_BGRA, GL_UNSIGNED_BYTE, FreeImage_GetBits( image )
	);
	
	//clamp each texture to the borders of whatever geometry holds it
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	
	// Create Mip Maps (if requested), then delete the FreeImage data,
	if ( imageFlags & CREATE_MIPMAPS ) {
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glGenerateMipmap( GL_TEXTURE_2D );
	}
	FreeImage_Unload( image );
	
	printGLError( fileName );
	glBindTexture( GL_TEXTURE_2D, 0 );
	return true;
}

void c_bitmap::unload() {
	if ( oglTexture != 0 )
		glDeleteTextures( 1, &oglTexture );
	oglTexture = 0; // insurance
	bmpWidth = 0;
	bmpHeight = 0;
}

//-----------------------------------------------------------------------------
//	Bitmap - Data Operations
//-----------------------------------------------------------------------------
void c_bitmap::makeActive( GLuint texUnit ) const {
	//glActiveTexture( texUnit );
	glBindTexture( GL_TEXTURE_2D, oglTexture );
}

void c_bitmap::deActivate() const {
	//glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, 0 );
}
