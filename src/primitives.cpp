/* 
 * File:   primitives.cpp
 * Author: hammy
 * 
 * Created on April 22, 2013, 8:40 PM
 */

#include <iostream>
#include "primitives.h"
#include "geometry.h"

namespace hge {

/******************************************************************************
 *      QUADS
******************************************************************************/
bool quad::init() {
	if ( vao )
        return true;
    
    bumpVertex verts[ 4 ];
    
    verts[0].pos = vec3( -0.5f, 0.5f, 0.f );
    verts[1].pos = vec3( 0.5f, 0.5f, 0.f );
    verts[2].pos = vec3( -0.5f, -0.5f, 0.f );
    verts[3].pos = vec3( 0.5f, -0.5f, 0.f );
    
    verts[0].uv = vec2( 0.f, 1.f );
    verts[1].uv = vec2( 1.f, 1.f );
    verts[2].uv = vec2( 0.f, 0.f );
    verts[3].uv = vec2( 1.f, 0.f );
    
    verts[0].norm = vec3( 0.f, 0.f, -1.f );
    verts[1].norm = vec3( 0.f, 0.f, -1.f );
    verts[2].norm = vec3( 0.f, 0.f, -1.f );
    verts[3].norm = vec3( 0.f, 0.f, -1.f );
    
    calcTangents( verts[0], verts[1], verts[2] );
    calcTangents( verts[1], verts[2], verts[3] );
	
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
    
    pipeline::enableBumpVertexAttribs();
	
	glBindVertexArray( 0 );
    
	return true;
}

void quad::terminate() {
    glDeleteVertexArrays( 1, &vao );
    glDeleteBuffers( 1, &vbo );

    vao = vbo = 0;
}

/******************************************************************************
 *      TRIANGLES
******************************************************************************/
bool triangle::init() {
	if ( vao )
        return true;
    
    bumpVertex verts[ 3 ];
    
    verts[0].pos = vec3( -0.5f, -0.5f, 0.f );
    verts[1].pos = vec3( 0.f, 0.5f, 0.f );
    verts[2].pos = vec3( 0.5f, -0.5f, 0.f );
    
    verts[0].uv = vec2( 0.f, 0.f );
    verts[1].uv = vec2( 0.5f, 1.f );
    verts[2].uv = vec2( 1.f, 0.f );
    
    verts[0].norm = vec3( 0.f, 0.f, -1.f );
    verts[1].norm = vec3( 0.f, 0.f, -1.f );
    verts[2].norm = vec3( 0.f, 0.f, -1.f );
    
    calcTangents( verts[0], verts[1], verts[2] );
	
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
    
    pipeline::enableBumpVertexAttribs();
	
	glBindVertexArray( 0 );
    
	return true;
}

void triangle::terminate() {
    glDeleteVertexArrays( 1, &vao );
    glDeleteBuffers( 1, &vbo );

    vao = vbo = 0;
}

/******************************************************************************
 *      LINES
******************************************************************************/
bool line::init() {
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
        GL_ARRAY_BUFFER, sizeof( line().points ),
        &line().points[0].v, GL_STREAM_DRAW
    );
    printGlError( "Initializing a line primitive" );
    glVertexAttribPointer( pipeline::VERTEX_ATTRIB, 3, GL_FLOAT, GL_FALSE, 0, 0 );
    glEnableVertexAttribArray( pipeline::VERTEX_ATTRIB );
    
    glDisableVertexAttribArray( pipeline::TEXTURE_ATTRIB );
    glDisableVertexAttribArray( pipeline::NORMAL_ATTRIB );
    glDisableVertexAttribArray( pipeline::TANGENT_ATTRIB );
    
    glBindVertexArray( 0 );
    return true;
}

void line::terminate() {
    glDeleteVertexArrays( 1, &vao );
    glDeleteBuffers( 1, &vbo );
    
    vao = vbo = 0;
}

void line::setVertPos( int index, const vec3& inPos ) {
    HGE_ASSERT( (index >= 0) && (index < 2) );
    points[ index ] = inPos;
    
    glBindVertexArray( vao );
    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glBufferData( GL_ARRAY_BUFFER, sizeof( points ), &points[0].v, GL_STREAM_DRAW );
}

/******************************************************************************
 *      CUBES
******************************************************************************/
bool cube::init() {
    
    if ( vao )
        return true;
    
    glGenVertexArrays( 1, &vao );
    if ( !vao ) {
        std::cerr << "ERROR: Unable to create a cube VAO." << std::endl;
        return false;
    }
    
    glBindVertexArray( vao );
    glGenBuffers( 1, &vbo );
    
    if ( !vbo ) {
        std::cerr << "ERROR: Unable to create a cube VBO." << std::endl;
        glDeleteVertexArrays( 1, &vao );
        return false;
    }
    
    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    
    const GLfloat verts[] = {
        // Front face
        -0.5f,-0.5f,0.5f, 0.5f,-0.5f,0.5f, -0.5f,0.5f,0.5f, 0.5f,0.5f,0.5f,
        // Right face
        0.5f,0.5f,0.5f, 0.5f,-0.5f,0.5f, 0.5f,0.5f,-0.5f, 0.5f,-0.5f,-0.5f,
        // Back face
        0.5f,-0.5f,-0.5f, -0.5f,-0.5f,-0.5f, 0.5f,0.5f,-0.5f, -0.5f,0.5f,-0.5f,
        // Left face
        -0.5f,0.5f,-0.5f, -0.5f,-0.5f,-0.5f, -0.5f,0.5f,0.5f, -0.5f,-0.5f,0.5f,
        // Bottom face
        -0.5f,-0.5f,0.5f, -0.5f,-0.5f,-0.5f, 0.5f,-0.5f,0.5f, 0.5f,-0.5f,-0.5f,
        // move to top
        0.5f,-0.5f,-0.5f, -0.5f,0.5f,0.5f,
        // Top Face
        -0.5f,0.5f,0.5f, 0.5f,0.5f,0.5f, -0.5f,0.5f,-0.5f, 0.5f,0.5f,-0.5f
    };
    
    const GLfloat uvs[] = {
        // Front face
        0.f,0.f, 1.f,0.f, 0.f,1.f, 1.f,1.f,
        // Right face
        0.f,1.f, 0.f,0.f, 1.f,1.f, 1.f,0.f,
        // Back face
        0.f,0.f, 1.f,0.f, 0.f,1.f, 1.f,1.f,
        // Left face
        0.f,1.f, 0.f,0.f, 1.f,1.f, 1.f,0.f,
        // Bottom face
        0.f,1.f, 0.f,0.f, 1.f,1.f, 1.f,0.f,
        1.f,0.f, 0.f,0.f,
        // Top face
        0.f,0.f, 1.f,0.f, 0.f,1.f, 1.f,1.f
	};

	const GLfloat norms[] = {
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};
    
    glBufferData(
        GL_ARRAY_BUFFER, sizeof( verts ) + sizeof( uvs ) + sizeof( norms ),
        nullptr, GL_STATIC_DRAW
    );
    
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof( verts ), verts );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof( verts ), sizeof( uvs ), uvs );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof( verts ) + sizeof( uvs ), sizeof( norms ), norms );
    
    glEnableVertexAttribArray( hge::pipeline::VERTEX_ATTRIB );
    glVertexAttribPointer(
        hge::pipeline::VERTEX_ATTRIB, 3, GL_FLOAT, GL_FALSE, 0, 0
    );
    
    glEnableVertexAttribArray( hge::pipeline::TEXTURE_ATTRIB );
    glVertexAttribPointer(
        hge::pipeline::TEXTURE_ATTRIB, 2, GL_FLOAT, GL_FALSE,
        0, (GLvoid*)sizeof( verts )
    );
    
    glEnableVertexAttribArray( hge::pipeline::NORMAL_ATTRIB );
    glVertexAttribPointer(
        hge::pipeline::NORMAL_ATTRIB, 3, GL_FLOAT, GL_FALSE,
        0, (GLvoid*)(sizeof( verts ) + sizeof( uvs ))
    );
    
    glBindVertexArray( 0 );
    printGlError( "Creating a cube object" );
    
    return true;
}

void cube::terminate() {
    glDeleteVertexArrays( 1, &vao );
    glDeleteBuffers( 1, &vbo );
    
    vao = vbo = 0;
}

/******************************************************************************
 *      SPHERES
******************************************************************************/
bool sphere::init( int rings, int sectors ) {
    float const R = 1.f / (float)(rings-1);
    float const S = 1.f / (float)(sectors-1);
    
    terminate();
    
    unsigned numVerts = rings * sectors;
    
    bumpVertex* vertices = new( std::nothrow ) bumpVertex[ rings * sectors ];
    if ( !vertices )
        return false;
        
    GLuint* indices = new( std::nothrow ) GLuint[ rings * sectors * 4 ];
    if ( !indices ) {
        delete [] vertices;
        vertices = nullptr;
        return false;
    }
    
    for( int r = 0, i = 0; r < rings; r++ ) {
        for( int s = 0; s < sectors; s++ ) {
            float const y = std::sin( -HL_PI_OVER_2 + HL_PI * r * R );
            float const x = std::cos( HL_TWO_PI * s * S ) * std::sin( HL_PI * r * R );
            float const z = std::sin( HL_TWO_PI * s * S ) * std::sin( HL_PI * r * R );
            
            vertices[ i ].uv[0] = s*S;
            vertices[ i ].uv[1] = r*R;
            
            vertices[ i ].pos[0] = x;
            vertices[ i ].pos[1] = y;
            vertices[ i ].pos[2] = z;
            
            vertices[ i ].norm[0] = x;
            vertices[ i ].norm[1] = y;
            vertices[ i ].norm[2] = z;
            ++i;
        }
    }
    
    for( int r = 0, i = 0; r < rings; r++ ) {
        for( int s = 0; s < sectors; s++ ) {
            indices[ i++ ] = r * sectors + s;
            indices[ i++ ] = (r+1) * sectors + s;
            indices[ i++ ] = r * sectors + (s+1);
            indices[ i++ ] = (r+1) * sectors + (s+1);
        }
    }
    
	glGenVertexArrays( 1, &vao );
	glBindVertexArray( vao );
	glGenBuffers( 2, vbo );
    
    if ( !vao || !vbo[0] || !vbo[1] ) {
        std::cerr
            << "An error occurred while initializing the sphere primitives"
            << std::endl;
        terminate();
        return false;
    }
    
    numIndices = numVerts * 4;
	
	glBindBuffer( GL_ARRAY_BUFFER, vbo[0] );
    glBufferData( GL_ARRAY_BUFFER, numVerts * sizeof( bumpVertex ), vertices, GL_STATIC_DRAW );
	printGlError( "Error while sending sphere primitive data to the GPU.");
    
    pipeline::enableBumpVertexAttribs();
    
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, vbo[1] );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof( GLuint ), indices, GL_STATIC_DRAW );
	
	glBindVertexArray( 0 );
    
    delete [] vertices;
    delete [] indices;
    
	return true;
}

void sphere::terminate() {
    glDeleteVertexArrays( 1, &vao );
    glDeleteBuffers( 2, vbo );

    vao = vbo[0] = vbo[1] = 0;
    numIndices = 0;
}

} // end hge namespace
