/* 
 * File:   bitmap.h
 * Author: hammy
 *
 * Created on November 20, 2012, 9:09 PM
 */

#ifndef __HGE_BITMAP_H__
#define	__HGE_BITMAP_H__

#include <FreeImage.h>
#include <GL/glew.h>
#include "types.h"
#include "resource.h"

//-----------------------------------------------------------------------------
//		OpenGL Bitmaps
//-----------------------------------------------------------------------------
class HGE_API c_bitmap : virtual public c_resource {
	private:
		GLuint oglTexture;
		int bmpWidth, bmpHeight;
		
		void	deduceImageFormat	( cstr fileName, FREE_IMAGE_FORMAT& ) const;
		void	setBitmapFlags		( FREE_IMAGE_FORMAT inFormat, int& outFlags );

	public:
		enum BMP_FLAGS {
			CREATE_MIPMAPS = 0x1
		};
		
		// con/destruction
		c_bitmap	();
		c_bitmap	( const c_bitmap& );
		~c_bitmap	();
		c_bitmap& operator = ( const c_bitmap& );
		
		// memory-based operations
		bool	isLoaded	() const;
		bool	load		( cstr filename, int flags = CREATE_MIPMAPS );
		void	unload		();
		
		// data operations
		uint	getWidth	() const	{ return bmpWidth; }
		uint	getHeight	() const	{ return bmpHeight; }
		GLuint	getTexID	() const	{ return oglTexture; }
		
		void	makeActive	( GLuint texUnit = GL_TEXTURE0 ) const;
		void	deActivate	() const;
};

#endif	/* __HGE_BITMAP_H__ */
