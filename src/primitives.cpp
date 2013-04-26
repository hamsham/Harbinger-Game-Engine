/* 
 * File:   primitives.cpp
 * Author: hammy
 * 
 * Created on April 22, 2013, 8:40 PM
 */

#include <iostream>
#include "primitives.h"

namespace hge {

/******************************************************************************
 *      PRIMITIVE SETUP
******************************************************************************/
bool initPrimitives() {
    if ( !c_quad::init() || !c_triangle::init() || !c_line::init() ) {
        terminatePrimitives();
        return false;
    }
    return true;
}

void terminatePrimitives() {
    c_quad::terminate();
    c_triangle::terminate();
    c_line::terminate();
}

/******************************************************************************
 *      STATIC MEMBER CONSTRUCTION
******************************************************************************/
GLuint c_quad::vao( 0 );
GLuint c_quad::vbo( 0 );
s_vertex c_quad::verts[ 4 ];

GLuint c_triangle::vao( 0 );
GLuint c_triangle::vbo( 0 );
s_vertex c_triangle::verts[ 3 ];

GLuint c_line::vao( 0 );
GLuint c_line::vbo( 0 );

/******************************************************************************
 *      QUADS
******************************************************************************/
bool c_quad::init() {
	if ( vao )
        return true;
    
    verts[0].pos = vec3( -1.f, 1.f, 0.f );
    verts[1].pos = vec3( 1.f, 1.f, 0.f );
    verts[2].pos = vec3( -1.f, -1.f, 0.f );
    verts[3].pos = vec3( 1.f, -1.f, 0.f );
    
    verts[0].uv = vec2( 0.f, 1.f );
    verts[1].uv = vec2( 1.f, 1.f );
    verts[2].uv = vec2( 0.f, 0.f );
    verts[3].uv = vec2( 1.f, 0.f );
    
    verts[0].norm = vec3( 0.f, 0.f, -1.f );
    verts[1].norm = vec3( 0.f, 0.f, -1.f );
    verts[2].norm = vec3( 0.f, 0.f, -1.f );
    verts[3].norm = vec3( 0.f, 0.f, -1.f );
    
    verts[0].tangent = vec3( 0.5f, 0.f, 0.5f );
    verts[1].tangent = vec3( 0.5f, 0.f, 0.5f );
    verts[2].tangent = vec3( 0.5f, 0.f, 0.5f );
    verts[3].tangent = vec3( 0.5f, 0.f, 0.5f );
	
	glGenVertexArrays( 1, &vao );
	glBindVertexArray( vao );
	glGenBuffers( 1, &vbo );
    
    if ( !vao || !vbo ) {
        std::cerr
            << "An error occurred while initializing the quad primitives"
            << std::endl;
        terminate();
        return false;
    }
	
	glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glBufferData( GL_ARRAY_BUFFER, sizeof( verts ), verts, GL_STATIC_DRAW );
	printGlError( "Error while sending primitive data to the GPU.");
    
	glEnableVertexAttribArray( pipeline::VERTEX_ATTRIB );
	glVertexAttribPointer(
		pipeline::VERTEX_ATTRIB,
		ARRAY_SIZE_FROM_ELEMENTS( s_vertex::pos.v ), GL_FLOAT, GL_FALSE,
        sizeof( s_vertex ), (GLvoid*)offsetof( s_vertex, pos.v )
	);
    
	glEnableVertexAttribArray( pipeline::TEXTURE_ATTRIB );
	glVertexAttribPointer(
		pipeline::TEXTURE_ATTRIB,
		ARRAY_SIZE_FROM_ELEMENTS( s_vertex::uv.v ), GL_FLOAT, GL_FALSE,
        sizeof( s_vertex ), (GLvoid*)offsetof( s_vertex, uv.v )
	);
    
	glEnableVertexAttribArray( pipeline::NORMAL_ATTRIB );
	glVertexAttribPointer(
		pipeline::NORMAL_ATTRIB,
		ARRAY_SIZE_FROM_ELEMENTS( s_vertex::norm.v ), GL_FLOAT, GL_FALSE,
        sizeof( s_vertex ), (GLvoid*)offsetof( s_vertex, norm.v )
	);
    
	glEnableVertexAttribArray( pipeline::TANGENT_ATTRIB );
	glVertexAttribPointer(
		pipeline::TANGENT_ATTRIB,
		ARRAY_SIZE_FROM_ELEMENTS( s_vertex::tangent.v ), GL_FLOAT, GL_FALSE,
        sizeof( s_vertex ), (GLvoid*)offsetof( s_vertex, tangent.v )
	);
	
	glBindVertexArray( 0 );
    
	return true;
}

void c_quad::terminate() {
    glDeleteVertexArrays( 1, &vao );
    glDeleteBuffers( 1, &vbo );

    vao = vbo = 0;
}

/******************************************************************************
 *      TRIANGLES
******************************************************************************/
bool c_triangle::init() {
	if ( vao )
        return true;
    
    verts[0].pos = vec3( -1.f, -1.f, 0.f );
    verts[1].pos = vec3( 0.f, 1.f, 0.f );
    verts[2].pos = vec3( 1.f, -1.f, 0.f );
    
    verts[0].uv = vec2( 0.f, 0.f );
    verts[1].uv = vec2( 0.5f, 1.f );
    verts[2].uv = vec2( 1.f, 0.f );
    
    verts[0].norm = vec3( 0.f, 0.f, -1.f );
    verts[1].norm = vec3( 0.f, 0.f, -1.f );
    verts[2].norm = vec3( 0.f, 0.f, -1.f );
    
    verts[0].tangent = vec3( 0.5f, 0.f, 0.5f );
    verts[1].tangent = vec3( 0.5f, 0.f, 0.5f );
    verts[2].tangent = vec3( 0.5f, 0.f, 0.5f );
	
	glGenVertexArrays( 1, &vao );
	glBindVertexArray( vao );
	glGenBuffers( 1, &vbo );
    
    if ( !vao || !vbo ) {
        std::cerr
            << "An error occurred while initializing the triangle primitives"
            << std::endl;
        terminate();
        return false;
    }
	
	glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glBufferData( GL_ARRAY_BUFFER, sizeof( verts ), verts, GL_STATIC_DRAW );
	printGlError( "Error while sending primitive data to the GPU.");
    
	glEnableVertexAttribArray( pipeline::VERTEX_ATTRIB );
	glVertexAttribPointer(
		pipeline::VERTEX_ATTRIB,
		ARRAY_SIZE_FROM_ELEMENTS( s_vertex::pos.v ), GL_FLOAT, GL_FALSE,
        sizeof( s_vertex ), (GLvoid*)offsetof( s_vertex, pos.v )
	);
    
	glEnableVertexAttribArray( pipeline::TEXTURE_ATTRIB );
	glVertexAttribPointer(
		pipeline::TEXTURE_ATTRIB,
		ARRAY_SIZE_FROM_ELEMENTS( s_vertex::uv.v ), GL_FLOAT, GL_FALSE,
        sizeof( s_vertex ), (GLvoid*)offsetof( s_vertex, uv.v )
	);
    
	glEnableVertexAttribArray( pipeline::NORMAL_ATTRIB );
	glVertexAttribPointer(
		pipeline::NORMAL_ATTRIB,
		ARRAY_SIZE_FROM_ELEMENTS( s_vertex::norm.v ), GL_FLOAT, GL_FALSE,
        sizeof( s_vertex ), (GLvoid*)offsetof( s_vertex, norm.v )
	);
    
	glEnableVertexAttribArray( pipeline::TANGENT_ATTRIB );
	glVertexAttribPointer(
		pipeline::TANGENT_ATTRIB,
		ARRAY_SIZE_FROM_ELEMENTS( s_vertex::tangent.v ), GL_FLOAT, GL_FALSE,
        sizeof( s_vertex ), (GLvoid*)offsetof( s_vertex, tangent.v )
	);
	
	glBindVertexArray( 0 );
    
	return true;
}

void c_triangle::terminate() {
    glDeleteVertexArrays( 1, &vao );
    glDeleteBuffers( 1, &vbo );

    vao = vbo = 0;
}

/******************************************************************************
 *      LINES
******************************************************************************/
bool c_line::init() {
    if ( vao )
        return true;
    
    glGenVertexArrays( 1, &vao );
    if ( !vao ) {
        std::cerr << "Unable to create a line VAO." << std::endl;
        return false;
    }
    
    glBindVertexArray( vao );
    glGenBuffers( 1, &vbo );
    
    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    
    glBufferData(
        GL_ARRAY_BUFFER, sizeof( c_line().points ),
        &c_line().points[0].v, GL_STREAM_DRAW
    );
    printGlError( "Initializing a line primitive" );
    glVertexAttribPointer( pipeline::VERTEX_ATTRIB, 3, GL_FLOAT, GL_FALSE, 0, 0 );
    glEnableVertexAttribArray( pipeline::VERTEX_ATTRIB );
    
    glBindVertexArray( 0 );
    return true;
}

void c_line::terminate() {
    glDeleteVertexArrays( 1, &vao );
    glDeleteBuffers( 1, &vbo );
    
    vao = vbo = 0;
}

void c_line::setVertPos( int index, const vec3& pos ) {
    HL_ASSERT( (index > 0) && (index < 2) );
    points[ index ] = pos;
    
    glBindVertexArray( vao );
    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glBufferData( GL_ARRAY_BUFFER, sizeof( points ), &points[0].v, GL_STREAM_DRAW );
}

void c_line::draw() const {
    glBindVertexArray( vao );
    glDrawArrays( GL_LINES, 0, 2 );
    glBindVertexArray( 0 );
}

} // end hge namespace
