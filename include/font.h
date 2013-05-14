/* 
 * File:   font.h
 * Author: hammy
 *
 * Created on April 1, 2013, 11:04 PM
 */

#ifndef __HGE_FONT_H__
#define	__HGE_FONT_H__

#include "mesh.h"
#include "math/math.h"

#ifndef HGE_MAX_NUM_GLYPHS
    #define HGE_MAX_NUM_GLYPHS 256
#endif

struct FT_FaceRec_;

namespace hge {

class HGE_API c_font : virtual public c_drawableObj, virtual public c_resource {
    
    struct charTexture {
        int width = 0;
        int height = 0;
        int advX = 0; // font advance
        int advY = 0; // font advance
        int bearX = 0; // font bearing
        unsigned textureId = 0;
        unsigned samplerId = 0;
    };
    
    private:
        static const int SPACES_PER_TAB = 5;
        
        GLuint vao = 0;
        GLuint vbo = 0;
//        GLuint ibo = 0;
        int newLine = 0;
        const char* text = HGE_NULL;
        charTexture charList[ HGE_MAX_NUM_GLYPHS ];
        
        void createCharBitmap( FT_FaceRec_*, int index, s_vertex* );
        void sendTexturesToGPU( charTexture&, const void* data );
        bool sendVerticesToGPU( const s_vertex* );
        void unloadCharTexture( charTexture& );
        
    public:
        c_font() {}
        ~c_font() { unload(); }
        
        bool load( const char* filename, int fontsize );
        bool isLoaded() const;
        void unload();
        
        void draw() const;
        
        void setText( const char* str ) { text = str; }
        const char* getText() const { return text; }
};

} // end hge namespace

#endif	/* __HGE_FONT_H__ */
