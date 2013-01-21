/* 
 * File:   image.cpp
 * Author: hammy
 * 
 * Created on January 20, 2013, 9:14 PM
 */

#include "image.h"
#include "shader.h"

const math::vec3 normalZ( math::vec3( 0.f, 0.f, 1.f ) );

c_image::c_image() :
	c_resource(),
	c_bitmap(),
	c_drawableObj(),
	vao( 0 ),
	vbo{ 0 },
	vert{
		-0.5f, 0.5f, 0.f,
		0.5f, 0.5f, 0.f,
		-0.5f, -0.5f, 0.f,
		0.5f, -0.5f, 0.f
	},
	uv{
		0.f, 1.f,
		1.f, 1.f,
		0.f, 0.f,
		1.f, 0.f
	}
{}

bool c_image::isLoaded() const {
	return vao != 0;
}

bool c_image::load( cstr filename, int flags ) {
	
	unload();
	
	if ( !c_bitmap::load( filename, flags ) ) {
		unload();
		return false;
	}
	
	glGenVertexArrays( 1, &vao );
	glBindVertexArray( vao );
	printGLError( "Image VAO error" );
	glGenBuffers( 2, vbo );
	
	glBindBuffer( GL_ARRAY_BUFFER, vbo[0] );
	glEnableVertexAttribArray( c_shader::VERTEX_ATTRIB );
	glBufferData( GL_ARRAY_BUFFER, sizeof( vert ), vert, GL_STATIC_DRAW );
	glVertexAttribPointer( c_shader::VERTEX_ATTRIB, 3, GL_FLOAT, GL_FALSE, 0, 0 );
	printGLError( "Image vertex error" );
	
	glBindBuffer( GL_ARRAY_BUFFER, vbo[1] );
	glEnableVertexAttribArray( c_shader::TEXTURE_ATTRIB );
	glBufferData( GL_ARRAY_BUFFER, sizeof( uv ), uv, GL_STATIC_DRAW );
	glVertexAttribPointer( c_shader::TEXTURE_ATTRIB, 2, GL_FLOAT, GL_FALSE,  0, 0 );
	printGLError( "Image uv error" );
	
	glDisableVertexAttribArray( c_shader::NORMAL_ATTRIB );
	
	glBindVertexArray( 0 );
	
	return true;
}

void c_image::unload() {
	c_bitmap::unload();
	if ( vao ) {
		glDeleteVertexArrays( 1, &vao );
		glDeleteBuffers( 2, vbo );
		
		vao = vbo[ 0 ] = vbo[ 1 ] = 0;
	}
}

void c_image::draw() const {
	glBindVertexArray( vao );
	c_bitmap::makeActive();
	glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
	c_bitmap::deActivate();
	glBindVertexArray( 0 );
}
