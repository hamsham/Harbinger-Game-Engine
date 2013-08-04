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
        GLuint      vao     = 0;
        GLuint      vbo     = 0;
        unsigned    numBmps = 10;

    public:
        billboard   ()              {}
        ~billboard  ()              { clearImages(); }
        
        void        clearImages     ();
        
        // Image positions will be reset upon calling "setNumImages()"
        bool        setNumImages    ( unsigned rows, unsigned cols );
        unsigned    getNumImages    () const { return numBmps; }
        
        void        setImagePos     ( unsigned index, const vec3& pos );
        
        // Ensure that a bitmap has been bound before drawing
        void        draw            () const;
        
        // Billboards only have one available draw mode and one attribute
        void        enableAttribute ( pipeline::attribute ) {}
        void        disableAttribute( pipeline::attribute ) {}
        void        setDrawMode     ( drawable::drawMode ) {}
};

} // end ge namespace

#endif	/* __HGE_BILLBOARD_H__ */
