/* 
 * File:   light.cpp
 * Author: hammy
 * 
 * Created on August 1, 2013, 9:41 PM
 */

#include <iostream>
#include <utility>
#include <vector>
#include <cstddef> /* used for offsetof */
#include "light.h"

#define DEFAULT_POINT_INTENSITY     1.f
#define DEFAULT_POINT_CONSTANT      0.f
#define DEFAULT_POINT_EXPONENTIAL   1.f
#define DEFAULT_POINT_LINEAR        0.f

namespace hge {

dsPointLight::dsPointLight() :
    scale( 1.f ),
    position( 0.f ),
    color( 1.f ),
    attributes(
        DEFAULT_POINT_CONSTANT,
        DEFAULT_POINT_EXPONENTIAL,
        DEFAULT_POINT_INTENSITY,
        DEFAULT_POINT_LINEAR
    )
{}

dsPointLight::dsPointLight( const dsPointLight& pl ) :
    scale( pl.scale ),
    position( pl.position ),
    color( pl.color ),
    attributes( pl.attributes )
{}

dsPointLight& dsPointLight::operator = ( const dsPointLight& pl ) {
    scale = pl.scale;
    position = pl.position;
    color = pl.color;
    attributes = pl.attributes;
    return *this;
}

float dsPointLight::calcInfluenceRadius() const {
    float maxChannel = hamLibs::math::max(
        hamLibs::math::max(
            color[0], color[1]
        ), color[2]
    );
    float c = maxChannel * attrib.intensity;
    return 8.f * HL_SQRT(c);
}

/******************************************************************************
 *      dsLightSphere vertex and index initialization
******************************************************************************/
std::vector< vec3 > subdivide( std::vector< vec3 >& inVerts ) {
    std::vector< vec3 > newVerts( inVerts.size() * 4 );
    
    for (unsigned i = 0, j = 0; i < inVerts.size(); i+=3) {
        vec3 a = normalize(( inVerts[i+0] + inVerts[i+1] ) * 0.5f);
        vec3 b = normalize(( inVerts[i+1] + inVerts[i+2] ) * 0.5f);
        vec3 c = normalize(( inVerts[i+2] + inVerts[i+0] ) * 0.5f);
        
        newVerts[ j++ ] = normalize(inVerts[i+0]);
        newVerts[ j++ ] = a;
        newVerts[ j++ ] = c;
        
        newVerts[ j++ ] = normalize(inVerts[i+1]);
        newVerts[ j++ ] = b;
        newVerts[ j++ ] = a;
        
        newVerts[ j++ ] = normalize(inVerts[i+2]);
        newVerts[ j++ ] = c;
        newVerts[ j++ ] = b;
        
        newVerts[ j++ ] = a;
        newVerts[ j++ ] = b;
        newVerts[ j++ ] = c;
    }
    return newVerts;
}
dsLightSphere::dsLightSphere( dsLightSphere&& ds ) :
    vao( ds.vao ),
    vbo{ ds.vbo[0], ds.vbo[1] },
    numInstances( ds.numInstances )
{
    ds.vao = 0;
    ds.vbo[0] = ds.vbo[1] = 0;
    ds.numInstances = 0;
}

dsLightSphere& dsLightSphere::operator = ( dsLightSphere&& ds ) {
    terminate();
    
    vao = ds.vao;
    ds.vao = 0;
    
    vbo[0] = ds.vbo[0];
    vbo[1] = ds.vbo[1];
    ds.vbo[0] = ds.vbo[1] = 0;
    
    numInstances = ds.numInstances;
    ds.numInstances = 0;
}

bool dsLightSphere::init() {
    terminate();
    std::vector< vec3 > vertices( 60 );
    
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
            vertices[i] = points[ indices[i] ];
        }
        
        vertices = std::move( subdivide( vertices ) );
    }
    
    if (!vao) {
        glGenVertexArrays( 1, &vao );
    }
    if (!vbo[0]) {
        glGenBuffers( 2, vbo );
    }
    
    if ( !vao || !vbo[0] || !vbo[1] ) {
        std::cerr
            << "An error occurred while initializing the sphere primitives"
            << std::endl;
        terminate();
        return false;
    }
    
	glBindVertexArray( vao );
	
	glBindBuffer( GL_ARRAY_BUFFER, vbo[0] );
    // Subdivided icosahedrons have been measured to use 240 vertices
    glBufferData( GL_ARRAY_BUFFER, sizeof(vec3)*240, vertices.data(), GL_STATIC_DRAW );
	printGlError( "Error while sending sphere primitive data to the GPU.");
    
	glEnableVertexAttribArray( pipeline::HGE_ATTRIB_VERTEX );
	glEnableVertexAttribArray( pipeline::HGE_ATTRIB_TEXTURE );
	glVertexAttribPointer(
		pipeline::HGE_ATTRIB_VERTEX, ARRAY_COUNT_FROM_SIZE( vec3::v ), GL_FLOAT,
        GL_FALSE, sizeof( vec3 ), (GLvoid*)offsetof( vec3, v )
	);
    
    /*
     * Begin Setting instanced attributes
     */
	glBindBuffer( GL_ARRAY_BUFFER, vbo[1] );
    glBufferData( GL_ARRAY_BUFFER, sizeof( dsPointLight ), nullptr, GL_DYNAMIC_DRAW );
    
    /* Set the scaling data location */
    glEnableVertexAttribArray( SCALE_ATTRIB );
	glEnableVertexAttribArray( POS_ATTRIB );
	glEnableVertexAttribArray( COLOR_ATTRIB );
	glEnableVertexAttribArray( PARAM_ATTRIB );
    
    glVertexAttribPointer(
        SCALE_ATTRIB, 1, GL_FLOAT, GL_FALSE, sizeof( dsPointLight ),
        (GLvoid*)offsetof( dsPointLight, dsPointLight::scale )
    );
    glVertexAttribDivisor( SCALE_ATTRIB, 1 );
    
    /* Set the position data location */
	glVertexAttribPointer(
		POS_ATTRIB, 3, GL_FLOAT, GL_FALSE, sizeof( dsPointLight ),
        (GLvoid*)offsetof( dsPointLight, dsPointLight::position )
	);
    glVertexAttribDivisor( POS_ATTRIB, 1 );
    
    /* Set the color data location */
	glVertexAttribPointer(
		COLOR_ATTRIB, 4, GL_FLOAT, GL_FALSE, sizeof( dsPointLight ),
        (GLvoid*)offsetof( dsPointLight, dsPointLight::color )
	);
    glVertexAttribDivisor( COLOR_ATTRIB, 1 );
    
    /* Set the light attribute data location */
	glVertexAttribPointer(
		PARAM_ATTRIB, 4, GL_FLOAT, GL_FALSE, sizeof( dsPointLight ),
        (GLvoid*)offsetof( dsPointLight, dsPointLight::attributes )
	);
    glVertexAttribDivisor( PARAM_ATTRIB, 1 );
    
    printGlError( "Error while setting light attribute information.");
	
	glBindVertexArray( 0 );
    
	return true;
}

/******************************************************************************
 *      dsLightSphere light initialization
******************************************************************************/
void dsLightSphere::setLightBuffer( const dsPointLight* lights, unsigned lightCount ) {
	glBindBuffer( GL_ARRAY_BUFFER, vbo[1] );
    glBufferData( GL_ARRAY_BUFFER, lightCount * sizeof( dsPointLight ), nullptr, GL_DYNAMIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, 0, lightCount * sizeof( dsPointLight ), lights );
	printGlError( "Error while uploading point light parameter data.");
    
    numInstances = lightCount;
}

/******************************************************************************
 *      dsLightSphere light Manipulation
******************************************************************************/
void dsLightSphere::setLight( unsigned index, const dsPointLight& pl ) {
    HL_ASSERT( index < numInstances );
    
	glBindBuffer( GL_ARRAY_BUFFER, vbo[1] );
    glBufferSubData(
        GL_ARRAY_BUFFER, index * sizeof( dsPointLight ),
        sizeof( dsPointLight ), &pl
    );
	printGlError( "Error while uploading point light data.");
}

/******************************************************************************
 *      dsLightSphere termination
******************************************************************************/
void dsLightSphere::terminate() {
    glDeleteVertexArrays( 1, &vao );
    vao = 0;
    glDeleteBuffers( 2, vbo );
    vbo[0] = vbo[1] = 0;
    
    numInstances = 0;
}

} // end hge namespace
