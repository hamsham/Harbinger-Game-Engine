/* 
 * File:   font.h
 * Author: hammy
 *
 * Created on April 1, 2013, 11:04 PM
 */

#ifndef __HGE_FONT_H__
#define	__HGE_FONT_H__

#include "resource.h"
#include "pipeline.h"
#include "drawable.h"

struct FT_FaceRec_;

namespace hge {

///////////////////////////////////////////////////////////////////////////////
// FONT TEXTURE ATLAS
///////////////////////////////////////////////////////////////////////////////
class HGE_API font final : public resource, public texture< GL_TEXTURE_2D > {
    
    friend class text;
    
    enum : int {
        SPACES_PER_TAB = 5,
        MAX_NUM_GLYPHS = 256
    };
    
    struct metric_t {
        int width   = 0;
        int height  = 0;
        int advX    = 0; // font advance
        int advY    = 0; // font advance
        int bearX   = 0; // font bearing
        
        vec2 pos;
        vec2 uv;
    };
    
    private:
        int         newLine     = 0;
        int         maxWidth    = 0;
        int         maxHeight   = 0;
        metric_t*   metrics     = nullptr;
        
        void        loadGlyphs( FT_FaceRec_* );
        void        createCharAtlas( GLubyte* bitmaps[ MAX_NUM_GLYPHS ] );
        
    public:
        
        font    () {}
        font    ( const font& ) = delete;
        font    ( font&& );
        
        ~font   ()           { unload(); }
        
        font&   operator =  ( const font& ) = delete;
        font&   operator =  ( font&& );
        
        bool    load        ( const char* filename, int fontsize );
        bool    isLoaded    () const;
        void    unload      ();
};

} // end hge namespace

#endif	/* __HGE_FONT_H__ */
