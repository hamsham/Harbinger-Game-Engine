/* 
 * File:   drawable.h
 * Author: hammy
 *
 * Created on August 2, 2013, 11:43 PM
 */

#ifndef __HGE_DRAWABLE_H__
#define	__HGE_DRAWABLE_H__

#include "pipeline.h"

namespace hge {

class HGE_API drawable {
    public:
        enum drawMode : GLenum {
            DRAW_DEFAULT = 0,
            DRAW_LINES = GL_LINES,
            DRAW_POINTS = GL_POINTS,
            DRAW_TRIANGLES = GL_TRIANGLES,
            DRAW_TRIANGLE_STRIP = GL_TRIANGLE_STRIP
        };
        
    protected:
        GLuint vao = 0;
        GLenum renderMode = DRAW_DEFAULT;
    
    public:
        drawable        () {}
        drawable        ( drawable&& );
        drawable        ( const drawable& ) = delete;
        
        virtual ~drawable() {}
        
        drawable&       operator =          ( drawable&& );
        drawable&       operator =          ( const drawable& ) = delete;
        // copying is removed as data should not be duplicated on the GPU
        
        virtual void    enableAttribute     ( pipeline::attribute );
        virtual void    disableAttribute    ( pipeline::attribute );
        
        void            setDrawMode         ( drawMode );
        virtual void    resetDrawMode       () = 0;
        drawMode        getDrawMode         () const { return (drawMode)renderMode; }
        
        virtual void    draw                () const = 0;
};

class HGE_API texture {
    protected:
        GLint textureUnit   = pipeline::HGE_TEXTURE_DEFAULT;
        unsigned textureId  = 0;
        
    public:
        texture         () {}
        texture         ( texture&& );
        texture         ( const texture& ) = delete;
        
        virtual ~texture() = 0;
        
        texture&        operator =  ( texture&& );
        texture&        operator =  ( const texture& ) = delete;
        // copying is removed as data should not be duplicated on the GPU
        
        virtual void    activate    () const = 0;
        virtual void    deActivate  () const = 0;
        
        virtual void    setTexUnit  ( GLint ) = 0;
};

} // end hge namespace

#endif	/* DRAWABLE_H */
