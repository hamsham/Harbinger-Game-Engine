/* 
 * File:   font.h
 * Author: hammy
 *
 * Created on April 1, 2013, 11:04 PM
 */

#ifndef __HGE_FONT_H__
#define	__HGE_FONT_H__

#include <vector>
#include "resource.h"
#include "pipeline.h"

struct FT_FaceRec_;

namespace hge {

///////////////////////////////////////////////////////////////////////////////
// FONT TEXTURE ATLAS
///////////////////////////////////////////////////////////////////////////////
class HGE_API font : virtual public resource {
    
    friend class string3d;
    
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
        GLint textureUnit   = pipeline::HGE_TEXTURE_DEFAULT;
        unsigned    textureId   = 0;
        int         newLine     = 0;
        int         maxWidth    = 0;
        int         maxHeight   = 0;
        metric_t    metrics[ MAX_NUM_GLYPHS ];
        
        void        loadGlyphs( FT_FaceRec_* );
        void        createCharAtlas( GLubyte* bitmaps[ MAX_NUM_GLYPHS ] );
        
    public:
        
        font  ()            {}
        ~font ()            { unload(); }
        
        font    ( const font& ) = delete;
        font    ( font&& ) = default;
        
        font&   operator =  ( const font& ) = delete;
        font&   operator =  ( font&& ) = default;
        bool    operator == ( const font& f ) { return textureId == f.textureId; }
        bool    operator != ( const font& f ) { return textureId != f.textureId; }
        
        bool    load        ( const char* filename, int fontsize );
        bool    isLoaded    () const;
        void    unload      ();
        
        void    activate    () const;
        void    deActivate  () const;
        
        void    setTexUnit  ( GLint texUnit = pipeline::HGE_TEXTURE_DEFAULT ) {
                                textureUnit = texUnit;
                            }
};

///////////////////////////////////////////////////////////////////////////////
// CHARACTER STRING CLASS
///////////////////////////////////////////////////////////////////////////////
class HGE_API string3d {
    typedef std::vector< int > intArr;
    
    private:
        unsigned    vao         = 0;
        unsigned    vbo         = 0;
        int         numChars    = 0;
        intArr      indices;
        // "indices" contains the starting elements of each character
        // plus the number of elements to draw (specified by an offset of "numChars")
        
        void        createVertexBuffer( unsigned numVerts );
        
    public:
        string3d()  {}
        ~string3d() { clearString(); }
        
        string3d    ( const string3d& ) = delete;
        string3d    ( string3d&& ) = default;
        string3d&   operator =  ( const string3d& ) = delete;
        string3d&   operator =  ( string3d&& ) = default;
        bool        operator == ( const string3d& s ) { return vao == s.vao; }
        bool        operator != ( const string3d& s ) { return vao != s.vao; }
        
        
        void        setString   ( const font&, const char* );
        void        clearString ();
        
        // USERS: ensure a font atlas has been activated before drawing
        void        draw        () const;
};

} // end hge namespace

#endif	/* __HGE_FONT_H__ */
