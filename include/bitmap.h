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
        uint oglTexture     = 0;
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
        ~c_bitmap   () { unload(); }
        c_bitmap&   operator = ( const c_bitmap& ) = delete;
        c_bitmap&   operator = ( c_bitmap&& );

        // memory-based operations
        bool    isLoaded    () const;
        bool    load        ( cstr filename, int flags = CREATE_MIPMAPS );
        void    unload      ();

        // data operations
        uint    getWidth    () const { return bmpWidth; }
        uint    getHeight   () const { return bmpHeight; }
        uint    getTexID    () const { return oglTexture; }

        void    makeActive  ( int texUnit = pipeline::HGE_TEXTURE_DEFAULT ) const;
        void    deActivate  () const;
};

/******************************************************************************
 *      3D Textures (Cube Maps)
******************************************************************************/
class HGE_API c_cubeMap {
    private:
        GLuint      textureObj      = 0;

    public:
        c_cubeMap   () {}
        c_cubeMap    ( const c_cubeMap& ) = delete;
        c_cubeMap    ( c_cubeMap&& );
        ~c_cubeMap  () { unload(); }
        c_cubeMap&  operator = ( const c_cubeMap& ) = delete;
        c_cubeMap&  operator = ( c_cubeMap&& );
        
        bool        loadTextures    ( const char* texFiles[ 6 ] );
        bool        isLoaded        () const { return textureObj != 0; }
        void        unload          ();
        void        activate        ( int texUnit = hge::pipeline::HGE_TEXTURE_DEFAULT ) const;
        void        deActivate      () const;
};

} // end hge namespace
#endif	/* __HGE_BITMAP_H__ */
