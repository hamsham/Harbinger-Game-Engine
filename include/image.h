/* 
 * File:   image.h
 * Author: hammy
 *
 * Created on January 20, 2013, 9:14 PM
 */

#ifndef __HGE_IMAGE_H__
#define	__HGE_IMAGE_H__

#include "bitmap.h"
#include "object.h"
#include "pipelineGL.h"

class HGE_API c_image :	virtual public c_resource,
				virtual public c_drawableObj {
	private:
		GLuint	vao;
		GLuint	vbo[ 2 ]; //vertex buffer, uv buffer
		GLfloat	vert[ 12 ];
		GLfloat	uv[ 12 ];
		c_bitmap bmp;
		
	public:
		c_image				();
		~c_image			() { unload(); }
		
		bool	isLoaded	() const;
		bool	load		( cstr filename, int flags = c_bitmap::CREATE_MIPMAPS );
		void	unload		();
		
		void	draw		() const;
};

#endif	/* __HGE_IMAGE_H__ */

