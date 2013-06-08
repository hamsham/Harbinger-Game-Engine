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
class HGE_API bitmap : virtual public resource {
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
        bitmap      () {}
        bitmap      ( const bitmap& ) = delete;
        bitmap      ( bitmap&& );
        ~bitmap     ()          { unload(); }
        
        bitmap&     operator =  ( const bitmap& ) = delete;
        bitmap&     operator =  ( bitmap&& );
        bool        operator == ( const bitmap& s ) { return oglTexture == s.oglTexture; }
        bool        operator != ( const bitmap& s ) { return oglTexture != s.oglTexture; }

        // memory-based operations
        bool        isLoaded    () const;
        bool        load        ( const char* filename, int unused = 0 );
        void        unload      ();

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
class HGE_API cubemap : virtual public resource {
    private:
        GLint       textureUnit     = pipeline::HGE_TEXTURE_DEFAULT;
        GLuint      textureObj      = 0;

    public:
        cubemap     () {}
        cubemap     ( const cubemap& ) = delete;
        cubemap     ( cubemap&& );
        ~cubemap    () { unload(); }
        
        cubemap&    operator =      ( const cubemap& ) = delete;
        cubemap&    operator =      ( cubemap&& );
        bool        operator ==     ( const cubemap& s ) { return textureObj == s.textureObj; }
        bool        operator !=     ( const cubemap& s ) { return textureObj != s.textureObj; }
        
        bool        load            ( const char* texFile, int cubeIndex );
        bool        isLoaded        () const { return textureObj != 0; }
        void        unload          ();
        
        void        activate        () const;
        void        deActivate      () const;
        
        void        setTexUnit      ( GLint texUnit = pipeline::HGE_TEXTURE_DEFAULT ) { textureUnit = texUnit; }
};

} // end hge namespace
#endif	/* __HGE_BITMAP_H__ */
