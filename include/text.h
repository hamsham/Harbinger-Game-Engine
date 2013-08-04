/* 
 * File:   text.h
 * Author: hammy
 *
 * Created on August 3, 2013, 7:15 PM
 */

#ifndef __HGE_TEXT_H__
#define	__HGE_TEXT_H__

#include <vector>
#include "pipeline.h"
#include "drawable.h"

namespace hge {
    
class font;

///////////////////////////////////////////////////////////////////////////////
// CHARACTER STRING CLASS
///////////////////////////////////////////////////////////////////////////////
class HGE_API text final : public drawable {
    typedef std::vector< int > intArr;
    
    private:
        unsigned    vbo         = 0;
        int         numChars    = 0;
        intArr      indices;
        // "indices" contains the starting elements of each character
        // plus the number of elements to draw (specified by an offset of "numChars")
        
        void        createVertexBuffer( unsigned numVerts );
        
    public:
        text    ()  {}
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
        void    draw                () const;
};

} // end hge namespace

#endif	/* __HGE_TEXT_H__ */

