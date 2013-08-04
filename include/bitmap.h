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
#include "drawable.h"

namespace hge {

/******************************************************************************
 *      2D Bitmaps
******************************************************************************/
class HGE_API bitmap final : public resource, public texture {
    private:
        int bmpWidth        = 0;
        int bmpHeight       = 0;

    public:

        enum BMP_FLAGS {
            CREATE_MIPMAPS = 0x1
        };

        // con/destruction
        bitmap      () = default;
        bitmap      ( const bitmap& ) = delete;
        bitmap      ( bitmap&& );
        ~bitmap     ()          { unload(); }
        
        bitmap&     operator =  ( const bitmap& ) = delete;
        bitmap&     operator =  ( bitmap&& );

        // memory-based operations
        bool        isLoaded    () const { return ( textureId != 0 ); }
        bool        load        ( const char* filename, int unused = 0 );
        void        unload      ();

        // data operations
        unsigned    getWidth    () const { return bmpWidth; }
        unsigned    getHeight   () const { return bmpHeight; }
        unsigned    getTexID    () const { return textureId; }

        void        activate    () const;
        void        deActivate  () const;
        
        inline void setTexUnit  ( GLint texUnit = pipeline::HGE_TEXTURE_DEFAULT );
};

inline void bitmap::setTexUnit( GLint texUnit ) {
    textureUnit = texUnit;
}

/******************************************************************************
 *      3D Textures (Cube Maps)
******************************************************************************/
class HGE_API cubemap final : public resource, public texture {
    public:
        cubemap     () {}
        cubemap     ( const cubemap& ) = delete;
        cubemap     ( cubemap&& );
        ~cubemap    () { unload(); }
        
        cubemap&    operator =      ( const cubemap& ) = delete;
        cubemap&    operator =      ( cubemap&& );
        
        bool        load            ( const char* texFile, int cubeIndex );
        bool        isLoaded        () const { return textureId != 0; }
        void        unload          ();
        
        void        activate        () const;
        void        deActivate      () const;
        
        void        setTexUnit      ( GLint texUnit = pipeline::HGE_TEXTURE_DEFAULT );
};

inline void cubemap::setTexUnit( GLint texUnit ) {
    textureUnit = texUnit;
}

} // end hge namespace
#endif	/* __HGE_BITMAP_H__ */
