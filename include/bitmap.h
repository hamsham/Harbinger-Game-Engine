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

//-----------------------------------------------------------------------------
//		OpenGL Bitmaps
//-----------------------------------------------------------------------------

class HGE_API c_bitmap : virtual public c_resource {
    private:
        uint oglTexture;
        int bmpWidth;
        int bmpHeight;

    public:

        enum BMP_FLAGS {
            CREATE_MIPMAPS = 0x1
        };

        // con/destruction
        c_bitmap    ();
        c_bitmap    ( const c_bitmap& );
        ~c_bitmap   ();
        c_bitmap&   operator = ( const c_bitmap& );

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

} // end hge namespace
#endif	/* __HGE_BITMAP_H__ */
