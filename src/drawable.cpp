/* 
 * File:   drawable.cpp
 * Author: hammy
 * 
 * Created on August 2, 2013, 11:43 PM
 */

#include "drawable.h"

namespace hge {

/******************************************************************************
 * Drawable Objects
******************************************************************************/
drawable::drawable( drawable&& d ) :
    vao( d.vao ),
    renderMode( d.renderMode )
{
    d.vao = 0;
    d.renderMode = 0;
}

drawable& drawable::operator=( drawable&& d ) {
    vao = d.vao;
    d.vao = 0;
    renderMode = d.renderMode;
    d.renderMode = 0;
    
    return *this;
}

void drawable::enableAttribute( pipeline::attribute a ) {
    HL_DEBUG_ASSERT(
        (a > pipeline::INVALID_UNIFORM) &&
        (a <= pipeline::BITANGENT_ATTRIB)
    );
    glBindVertexArray( vao );
    glEnableVertexAttribArray( (GLint)a );
    glBindVertexArray( 0 );
}

void drawable::disableAttribute( pipeline::attribute a ) {
    HL_DEBUG_ASSERT(
        (a > pipeline::INVALID_UNIFORM) &&
        (a <= pipeline::BITANGENT_ATTRIB)
    );
    glBindVertexArray( vao );
    glDisableVertexAttribArray( (GLint)a );
    glBindVertexArray( 0 );
}

void drawable::setDrawMode( drawMode d ) {
    if ( d == DRAW_DEFAULT )
        resetDrawMode();
    else
        renderMode = (GLenum)d;
}

/******************************************************************************
 * Texture Objects
******************************************************************************/
texture::texture( texture&& t ) :
    textureUnit( t.textureUnit ),
    textureId( t.textureId )
{
    t.textureUnit = 0;
    t.textureId = 0;
}

texture& texture::operator=( texture&& t ) {
    textureUnit = t.textureUnit;
    t.textureUnit = 0;
    textureId = t.textureId;
    t.textureId = 0;
    
    return *this;
}

texture::~texture() {
}

} // end hge namespace
