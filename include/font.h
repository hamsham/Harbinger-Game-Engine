/* 
 * File:   font.h
 * Author: hammy
 *
 * Created on April 1, 2013, 11:04 PM
 */

#ifndef __HGE_FONT_H__
#define	__HGE_FONT_H__

#include <vector>

#include "types.h"
#include "object.h"
#include "resource.h"
#include "math/math.h"

struct FT_FaceRec_;

namespace hge {

class HGE_API c_font : virtual public c_drawableObj, virtual public c_resource {

    typedef std::vector< unsigned char > vboArray;
    
    struct charTexture {
        int width = 0;
        int height = 0;
        int advX = 0; // font advance
        int bearX = 0; // font bearing
        unsigned textureId = 0;
        unsigned samplerId = 0;
    };
    
    private:
        static const int SPACES_PER_TAB = 5;
        
        int vao = 0;
        int vbo = 0;
        int newLine = 0;
        int lineHeight = 0;
        const char* text = HGE_NULL;
        charTexture charList[ 256 ];
        
        void createCharBitmap( FT_FaceRec_*, int index, vboArray& );
        void generateCharTexture( charTexture&, int w, int h, const void* data );
        void unloadCharTexture( charTexture& );
        
    public:
        c_font() {}
        ~c_font() { unload(); }
        
        bool load( const char* filename, int fontsize );
        bool isLoaded() const;
        void unload();
        
        //void draw( const char*, float size, GLuint, const hamLibs::math:: mat4& );
        //void drawOrtho( const char*, float size, GLuint );
        void draw() const;
        
        void setText( const char* str ) { text = str; }
        const char* getText() const { return text; }
};

} // end hge namespace

#endif	/* __HGE_FONT_H__ */
