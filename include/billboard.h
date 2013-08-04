/* 
 * File:   billboard.h
 * Author: hammy
 *
 * Created on May 20, 2013, 12:14 AM
 */

#ifndef __HGE_BILLBOARD_H__
#define	__HGE_BILLBOARD_H__

#include "pipeline.h"
#include "drawable.h"

namespace hge {

class HGE_API billboard final : public drawable {
    private:
        GLuint      vbo     = 0;
        unsigned    numBmps = 0;

    public:
        billboard   () {}
        billboard   ( const billboard& ) = delete;
        billboard   ( billboard&& );
        
        ~billboard  ()              { clearImages(); }
        
        billboard&  operator =      ( const billboard& ) = delete;
        billboard&  operator =      ( billboard&& );
        
        void        clearImages     ();
        
        // Image positions will be reset upon calling "setNumImages()"
        bool        setNumImages    ( unsigned rows, unsigned cols );
        unsigned    getNumImages    () const { return numBmps; }
        
        void        setImagePos     ( unsigned index, const vec3& pos );
        
        // Billboards only have one available draw mode and one attribute
        void        enableAttribute ( pipeline::attribute ) override {}
        void        disableAttribute( pipeline::attribute ) override {}
        
        // Ensure that a bitmap has been bound before drawing
        void        resetDrawMode   () override { renderMode = pipeline::HGE_POINTS; }
        void        draw            () const {
                                        glBindVertexArray( vao );
                                        glDrawArrays( pipeline::HGE_POINTS, 0, numBmps );
                                        glBindVertexArray( 0 );
                                    }
};

} // end ge namespace

#endif	/* __HGE_BILLBOARD_H__ */
