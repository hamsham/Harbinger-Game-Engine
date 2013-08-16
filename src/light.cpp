/* 
 * File:   light.cpp
 * Author: hammy
 * 
 * Created on August 1, 2013, 9:41 PM
 */

#include <iostream>
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
dsLightSphere::dsLightSphere( dsLightSphere&& ds ) :
    vao( ds.vao ),
    vbo{ ds.vbo[0], ds.vbo[1], ds.vbo[2] },
    numIndices( ds.numIndices ),
    numInstances( ds.numInstances )
{
    ds.vao = 0;
    ds.vbo[0] = ds.vbo[1] = ds.vbo[2] = 0;
    ds.numIndices = 0;
    ds.numInstances = 0;
}

dsLightSphere& dsLightSphere::operator = ( dsLightSphere&& ds ) {
    vao = ds.vao;
    ds.vao = 0;
    
    vbo[0] = ds.vbo[0];
    vbo[1] = ds.vbo[1];
    vbo[2] = ds.vbo[2];
    ds.vbo[0] = ds.vbo[1] = ds.vbo[2] = 0;
    
    numIndices = ds.numIndices;
    ds.numIndices = 0;
    
    numInstances = ds.numInstances;
    ds.numInstances = 0;
}

bool dsLightSphere::init() {
    
    terminate();
    
    const int rings = DEFAULT_NUM_RINGS;
    const int sectors = DEFAULT_NUM_SECTORS;
    float const R = 1.f / (float)(rings-1);
    float const S = 1.f / (float)(sectors-1);
    
    unsigned numVerts = rings * sectors;
    
    vec3* vertices = new( std::nothrow ) vec3[ rings * sectors ];
    if ( !vertices )
        return false;
        
    GLuint* indices = new( std::nothrow ) GLuint[ rings * sectors * 4 ];
    if ( !indices ) {
        delete [] vertices;
        vertices = nullptr;
        return false;
    }
    
    for( int r = 0, i = 0, j = 0; r < rings; r++ ) {
        for( int s = 0; s < sectors; s++ ) {
            float const y = std::sin( -HL_PI_OVER_2 + HL_PI * r * R );
            float const x = std::cos( HL_TWO_PI * s * S ) * std::sin( HL_PI * r * R );
            float const z = std::sin( HL_TWO_PI * s * S ) * std::sin( HL_PI * r * R );
            
            vertices[ i ][0] = x;
            vertices[ i ][1] = y;
            vertices[ i ][2] = z;
            ++i;
            
            indices[ j++ ] = r * sectors + s;
            indices[ j++ ] = (r+1) * sectors + s;
            indices[ j++ ] = r * sectors + (s+1);
            indices[ j++ ] = (r+1) * sectors + (s+1);
        }
    }
    
	glGenVertexArrays( 1, &vao );
	glBindVertexArray( vao );
	glGenBuffers( 3, vbo );
    
    if ( !vao || !vbo[0] || !vbo[1] || !vbo[2] ) {
        std::cerr
            << "An error occurred while initializing the sphere primitives"
            << std::endl;
        terminate();
        return false;
    }
    
    numIndices = numVerts * 4;
	
	glBindBuffer( GL_ARRAY_BUFFER, vbo[0] );
    glBufferData( GL_ARRAY_BUFFER, numVerts * sizeof( vec3 ), vertices, GL_STATIC_DRAW );
	printGlError( "Error while sending sphere primitive data to the GPU.");
    
	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( vec3 ), 0 );
    glVertexAttribDivisor( 0, 0 );
    
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, vbo[1] );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof( GLuint ), indices, GL_STATIC_DRAW );
    
    delete [] vertices;
    delete [] indices;
    
    /*
     * Begin Setting instanced attributes
     */
	glBindBuffer( GL_ARRAY_BUFFER, vbo[2] );
    glBufferData( GL_ARRAY_BUFFER, sizeof( dsPointLight ), nullptr, GL_STREAM_DRAW );
    
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
	glBindBuffer( GL_ARRAY_BUFFER, vbo[2] );
    
    glBufferData(
        GL_ARRAY_BUFFER, lightCount * sizeof( dsPointLight ), lights, GL_DYNAMIC_DRAW
    );
	printGlError( "Error while uploading point light parameter data.");
    
    numInstances = lightCount;
}

/******************************************************************************
 *      dsLightSphere light Manipulation
******************************************************************************/
void dsLightSphere::setLight( unsigned index, const dsPointLight& pl ) {
    HL_ASSERT( index < numInstances );
    
	glBindBuffer( GL_ARRAY_BUFFER, vbo[2] );
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
    glDeleteBuffers( 3, vbo );
    vbo[0] = vbo[1] = vbo[2] = 0;
    
    numIndices = numInstances = 0;
}

} // end hge namespace
