/* 
 * File:   skybox.cpp
 * Author: hammy
 * 
 * Created on April 25, 2013, 6:42 PM
 */

#include <iostream>
#include <utility>
#include <vector>
#include <cstddef> /* used for offsetof */
#include "skybox.h"

struct skyVert {
    hamLibs::math::vec3 pos;
    hamLibs::math::vec2 uv;
};

std::vector< skyVert > subdivide( std::vector< skyVert >& inVerts ) {
    std::vector< skyVert > newVerts( inVerts.size() * 4 );
    
    for (unsigned i = 0, j = 0; i < inVerts.size(); i+=3) {
        skyVert a;
        skyVert b;
        skyVert c;
        
        a.pos = ( inVerts[i+0].pos + inVerts[i+1].pos ) * 0.5f;
        b.pos = ( inVerts[i+1].pos + inVerts[i+2].pos ) * 0.5f;
        c.pos = ( inVerts[i+2].pos + inVerts[i+0].pos ) * 0.5f;
        
        newVerts[ j++ ] = inVerts[i+0];
        newVerts[ j++ ] = a;
        newVerts[ j++ ] = c;
        
        newVerts[ j++ ] = inVerts[i+1];
        newVerts[ j++ ] = b;
        newVerts[ j++ ] = a;
        
        newVerts[ j++ ] = inVerts[i+2];
        newVerts[ j++ ] = c;
        newVerts[ j++ ] = b;
        
        newVerts[ j++ ] = a;
        newVerts[ j++ ] = b;
        newVerts[ j++ ] = c;
    }
    return newVerts;
}

namespace hge {

skybox::skybox( skybox&& s ) :
    drawable( std::move( s ) ),
    skyTex( std::move( s.skyTex ) ),
    vbo( s.vbo ),
    numTris( s.numTris )
{
    s.vbo = 0;
    s.numTris = 0;
}

skybox& skybox::operator =( skybox&& s ) {
    drawable::operator =( std::move( s ) );
    
    skyTex = std::move( s.skyTex );
    
    vbo = s.vbo;
    s.vbo = 0;
    
    numTris = s.numTris;
    s.numTris = 0;
}
void skybox::enableAttribute( pipeline::attribute a ) {
    if (    a != pipeline::HGE_ATTRIB_VERTEX
    ||      a != pipeline::HGE_ATTRIB_TEXTURE
    ) {
        return;
    }
    
    glBindVertexArray( vao );
    glEnableVertexAttribArray( (GLint)a );
    glBindVertexArray( 0 );
}

void skybox::disableAttribute( pipeline::attribute a ) {
    if (    a != pipeline::HGE_ATTRIB_VERTEX
    ||      a != pipeline::HGE_ATTRIB_TEXTURE
    ) {
        return;
    }
    
    glBindVertexArray( vao );
    glEnableVertexAttribArray( (GLint)a );
    glBindVertexArray( 0 );
}

void skybox::unload() {
    skyTex.unload();
    
    glDeleteVertexArrays( 1, &vao );
    glDeleteBuffers( 1, &vbo );
    
    vao = vbo = 0;
    numTris = 0;
}

bool skybox::load( const char* skyFiles[ 6 ] ) {
    if (    !skyTex.load( skyFiles[ 0 ], 0 )
    ||      !skyTex.load( skyFiles[ 1 ], 1 )
    ||      !skyTex.load( skyFiles[ 2 ], 2 )
    ||      !skyTex.load( skyFiles[ 3 ], 3 )
    ||      !skyTex.load( skyFiles[ 4 ], 4 )
    ||      !skyTex.load( skyFiles[ 5 ], 5 )
    ) {
        skyTex.unload();
        return false;
    }
    
    std::vector< skyVert > vertices( 60 );
    
    {
        std::vector< GLuint > indices = {
            0,11,5,     0,5,1,      0,1,7,      0,7,10,     0,10,11,
            1,5,9,      5,11,4,     11,10,2,    10,7,6,     7,1,8,
            3,9,4,      3,4,2,      3,2,6,      3,6,8,      3,8,9,
            4,9,5,      2,4,11,     6,2,10,     8,6,7,      9,8,1
        }; // 60 elements
        // build an icosahedron

        const float t = (1.f + sqrt(5.f))/2.f;
        //const float s = 1.f / sqrt(1.f + t*t);

        // create the 12 vertices
        std::vector< vec3 > points = {
            vec3(-1.f, t, 0.f),     vec3(1.f, t, 0.f),
            vec3(-1.f, -t, 0.f),    vec3(1.f, -t, 0.f),
            vec3(0.f, -1.f, t),     vec3(0.f, 1.f, t),
            vec3(0.f, -1.f, -t),    vec3(0.f, 1.f, -t),
            vec3(t, 0.f, -1.f),     vec3(t, 0.f, 1.f),
            vec3(-t, 0.f, -1.f),    vec3(-t, 0.f, 1.f)
        };

        // create 20 triangles (60 vertices)
        for( unsigned i = 0; i < indices.size(); ++i ) {
            vertices[i].pos = points[ indices[i] ];
        }
        
        vertices = std::move( subdivide( vertices ) );
    }
    
    // Populate the UV/Normal/Tangent Data
    for ( unsigned i = 0; i < vertices.size(); ++i ) {
        skyVert& vert = vertices[i];
        vert.pos = normalize( vert.pos );
        vert.uv = vec2(
            std::atan2(vert.pos[0], vert.pos[2])/HL_TWO_PI + 0.5f,
            vert.pos[1] * 0.5f + 0.5f
        );
    }
    
    if (!vao) {
        glGenVertexArrays( 1, &vao );
    }
    if (!vbo) {
        glGenBuffers( 1, &vbo );
    }
    
    if ( !vao || !vbo ) {
        std::cerr
            << "An error occurred while initializing the sphere primitives"
            << std::endl;
        unload();
        return false;
    }
    
    numTris = vertices.size();
    
	glBindVertexArray( vao );
	
	glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glBufferData( GL_ARRAY_BUFFER, sizeof(skyVert)*numTris, vertices.data(), GL_STATIC_DRAW );
	printGlError( "Error while sending sphere primitive data to the GPU.");
    
	glEnableVertexAttribArray( pipeline::HGE_ATTRIB_VERTEX );
	glEnableVertexAttribArray( pipeline::HGE_ATTRIB_TEXTURE );
	glVertexAttribPointer(
		pipeline::HGE_ATTRIB_VERTEX, ARRAY_COUNT_FROM_SIZE( skyVert::pos.v ), GL_FLOAT,
        GL_FALSE, sizeof( skyVert ), (GLvoid*)offsetof( skyVert, pos.v )
	);
    
	glVertexAttribPointer(
		pipeline::HGE_ATTRIB_TEXTURE, ARRAY_COUNT_FROM_SIZE( skyVert::uv.v ), GL_FLOAT,
        GL_FALSE, sizeof( skyVert ), (GLvoid*)offsetof( skyVert, uv.v )
	);
    
	glBindVertexArray( 0 );
    
    resetDrawMode();
    
	return true;
}

} // End hge namespace
