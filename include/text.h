/* 
 * File:   text.h
 * Author: hammy
 *
 * Created on August 3, 2013, 7:15 PM
 */

#ifndef __HGE_TEXT_H__
#define	__HGE_TEXT_H__

#include "pipeline.h"
#include "drawable.h"

namespace hge {
    
class font;

///////////////////////////////////////////////////////////////////////////////
// CHARACTER STRING CLASS
///////////////////////////////////////////////////////////////////////////////
class HGE_API text final : public drawable {
    private:
        unsigned    vbo         = 0;
        int         numChars    = 0;
        GLint*      indices     = nullptr;
        // "indices" contains the starting elements of each character
        // plus the number of elements to draw (specified by an offset of "numChars")
        
        void        createVertexBuffer( const char* str );
        
    public:
        text    ()  { resetDrawMode(); }
        text    ( const text& ) = delete;
        text    ( text&& );
        
        ~text   () { clearString(); }
        
        text&   operator =          ( const text& ) = delete;
        text&   operator =          ( text&& );
        
        void    setString           ( const font&, const char* );
        void    clearString         ();
        
        // A font atlas must be set before drawing
        void    enableAttribute     ( pipeline::attribute ) override;
        void    disableAttribute    ( pipeline::attribute ) override;
        
        void    resetDrawMode       () override { renderMode = pipeline::HGE_TRIANGLE_STRIP; }
        
        // Blending MUST be enabled for text to render properly
        // It is defaulted to be off in order to allow client applications to
        // choose which render method is best suited for it
        void    draw                () const;
};

inline void text::draw() const {
    // glEnable( GL_BLEND );
    
    /* Premultiplied alpha */
    // glBlendEquationSeparate( GL_FUNC_ADD, GL_FUNC_ADD );
    // glBlendFuncSeparate( GL_ONE, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO );
    
    /* Additive Blending */
    //  glBlendFunc( GL_ONE, GL_ONE );
    
    glBindVertexArray( vao );
    glMultiDrawArrays(
        renderMode, indices,
        (const GLsizei*)(indices + numChars), numChars
    );
    glBindVertexArray( 0 );
    
    // glDisable( GL_BLEND );
}

} // end hge namespace

#endif	/* __HGE_TEXT_H__ */

