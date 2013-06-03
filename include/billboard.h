/* 
 * File:   billboard.h
 * Author: hammy
 *
 * Created on May 20, 2013, 12:14 AM
 */

#ifndef __HGE_BILLBOARD_H__
#define	__HGE_BILLBOARD_H__

#include "object.h"
#include "pipeline.h"

namespace hge {

class HGE_API c_billboard {
    private:
        GLuint      vao     = 0;
        GLuint      vbo     = 0;
        unsigned    numBmps = 10;

    public:
        c_billboard ()              {}
        ~c_billboard()              { clearInstances(); }
        
        void        clearInstances  ();
        
        bool        setNumInstances ( unsigned rows, unsigned cols );
        unsigned    getNumInstances () const { return numBmps; }
        
        void        setInstancePos  ( unsigned index, const vec3& pos );
        
        // Ensure that a bitmap has been bound before drawing
        void        draw            () const;
};

} // end ge namespace

#endif	/* __HGE_BILLBOARD_H__ */
