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

/******************************************************************************
 * Drawable object base class
******************************************************************************/
class HGE_API drawable {
    protected:
        GLuint vao = 0;
        GLenum renderMode = pipeline::HGE_DEFAULT;
    
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
        
        void            setDrawMode         ( pipeline::drawMode );
        virtual void    resetDrawMode       () = 0;
        pipeline::drawMode getDrawMode      () const { return (pipeline::drawMode)renderMode; }
        
        virtual void    draw                () const = 0;
};

/******************************************************************************
 * texture object base class
******************************************************************************/
template <int texType = GL_TEXTURE_2D>
class HGE_API texture {
    protected:
        int textureUnit   = pipeline::HGE_TEXTURE_DEFAULT;
        unsigned textureId  = 0;
        
    public:
        texture     () {}
        texture     ( texture&& );
        texture     ( const texture& ) = delete;
        
        virtual ~texture        () = 0;
        
        texture&    operator =  ( texture&& );
        texture&    operator =  ( const texture& ) = delete;
        // copying is removed as data should not be duplicated on the GPU
        
        void        activate    () const;
        void        deActivate  () const;
        
        void        setTexUnit  ( int t = pipeline::HGE_TEXTURE_DEFAULT ) {
                                    textureUnit = t;
                                }
};

template < int texType >
texture< texType >::~texture() {}

template < int texType >
texture< texType >::texture( texture< texType >&& t ) :
    textureUnit( t.textureUnit ),
    textureId( t.textureId )
{
    t.textureUnit = 0;
    t.textureId = 0;
}

template < int texType >
texture< texType >& texture< texType >::operator=( texture< texType >&& t ) {
    textureUnit = t.textureUnit;
    t.textureUnit = 0;
    textureId = t.textureId;
    t.textureId = 0;
    
    return *this;
}

// texture binding
template < int texType >
void texture< texType >::activate() const {
    glActiveTexture( textureUnit );
    glBindTexture( texType, textureId );
}

template < int texType >
void texture< texType >::deActivate() const {
	glActiveTexture( textureUnit );
	glBindTexture( texType, 0 );
}

} // end hge namespace

#endif	/* DRAWABLE_H */
