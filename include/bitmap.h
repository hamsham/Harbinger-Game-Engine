/* 
 * File:   bitmap.h
 * Author: hammy
 *
 * Created on November 20, 2012, 9:09 PM
 */

#ifndef __HGE_BITMAP_H__
#define	__HGE_BITMAP_H__

#include "pipeline.h"
#include "resource.h"

namespace hge {

/******************************************************************************
 *      2D Bitmaps
******************************************************************************/
class HGE_API c_bitmap : virtual public c_resource {
    private:
        GLint textureUnit   = pipeline::HGE_TEXTURE_DEFAULT;
        unsigned oglTexture = 0;
        int bmpWidth        = 0;
        int bmpHeight       = 0;

    public:

        enum BMP_FLAGS {
            CREATE_MIPMAPS = 0x1
        };

        // con/destruction
        c_bitmap    () {}
        c_bitmap    ( const c_bitmap& ) = delete;
        c_bitmap    ( c_bitmap&& );
        ~c_bitmap   ()          { unload(); }
        
        c_bitmap&   operator =  ( const c_bitmap& ) = delete;
        c_bitmap&   operator =  ( c_bitmap&& );
        bool        operator == ( const c_bitmap& s ) { return oglTexture == s.oglTexture; }
        bool        operator != ( const c_bitmap& s ) { return oglTexture != s.oglTexture; }

        // memory-based operations
        bool    isLoaded        () const;
        bool    load            ( const char* filename, int unused = 0 );
        void    unload          ();

        // data operations
        unsigned    getWidth    () const { return bmpWidth; }
        unsigned    getHeight   () const { return bmpHeight; }
        unsigned    getTexID    () const { return oglTexture; }

        void        activate    () const;
        void        deActivate  () const;
        
        void        setTexUnit  ( GLint texUnit = pipeline::HGE_TEXTURE_DEFAULT ) {
                                    textureUnit = texUnit;
                                }
};

/******************************************************************************
 *      3D Textures (Cube Maps)
******************************************************************************/
class HGE_API c_cubeMap : virtual public c_resource {
    private:
        GLint       textureUnit     = pipeline::HGE_TEXTURE_DEFAULT;
        GLuint      textureObj      = 0;

    public:
        c_cubeMap   () {}
        c_cubeMap   ( const c_cubeMap& ) = delete;
        c_cubeMap   ( c_cubeMap&& );
        ~c_cubeMap  () { unload(); }
        
        c_cubeMap&  operator =      ( const c_cubeMap& ) = delete;
        c_cubeMap&  operator =      ( c_cubeMap&& );
        bool        operator ==     ( const c_cubeMap& s ) { return textureObj == s.textureObj; }
        bool        operator !=     ( const c_cubeMap& s ) { return textureObj != s.textureObj; }
        
        bool        load            ( const char* texFile, int cubeIndex );
        bool        isLoaded        () const { return textureObj != 0; }
        void        unload          ();
        
        void        activate        () const;
        void        deActivate      () const;
        
        void        setTexUnit      ( GLint texUnit = pipeline::HGE_TEXTURE_DEFAULT ) { textureUnit = texUnit; }
};

} // end hge namespace
#endif	/* __HGE_BITMAP_H__ */
