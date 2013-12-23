/* 
 * File:   primitives.cpp
 * Author: hammy
 * 
 * Created on April 22, 2013, 8:40 PM
 */

#include <iostream>
#include <utility>
#include <vector>
#include "primitives.h"
#include "geometry.h"

namespace hge {

/******************************************************************************
 *      PRIMITIVE BASE CLASS
******************************************************************************/
primitive::primitive( primitive&& p ) :
    drawable( std::move( p ) ),
    vbo( p.vbo )
{
    p.vbo = 0;
}

primitive& primitive::operator = ( primitive&& p ) {
    terminate();
    
    drawable::operator=( std::move( p ) );
    vbo = p.vbo;
    p.vbo = 0;
    
    return *this;
}

primitive::~primitive() {}

void primitive::terminate() {
    glDeleteVertexArrays( 1, &vao );
    glDeleteBuffers( 1, &vbo );
    
    vao = vbo = 0;
}

/******************************************************************************
 *      QUADS
******************************************************************************/
bool quad::init() {
	if ( vao )
        return true;
    
    bumpVertex verts[ 4 ];
    
    /* 1----0
     * | \\ |
     * 3----2 */
    verts[0].pos = vec3( 0.5f, 0.5f, 0.f );
    verts[1].pos = vec3( -0.5f, 0.5f, 0.f );
    verts[2].pos = vec3( 0.5f, -0.5f, 0.f );
    verts[3].pos = vec3( -0.5f, -0.5f, 0.f );
    
    verts[0].uv = vec2( 1.f, 1.f );
    verts[1].uv = vec2( 0.f, 1.f );
    verts[2].uv = vec2( 1.f, 0.f );
    verts[3].uv = vec2( 0.f, 0.f );
    
    verts[0].norm =
        verts[1].norm =
            verts[2].norm =
                verts[3].norm = vec3( 0.f, 0.f, 1.f );
    
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
    
    resetDrawMode();
    
	return true;
}

/******************************************************************************
 *      TRIANGLES
******************************************************************************/
bool triangle::init() {
	if ( vao )
        return true;
    
    bumpVertex verts[ 3 ];
    
    verts[0].pos = vec3( 0.f, 0.5f, 0.f );
    verts[1].pos = vec3( -0.5f, -0.5f, 0.f );
    verts[2].pos = vec3( 0.5f, -0.5f, 0.f );
    
    verts[0].uv = vec2( 0.5f, 1.f );
    verts[1].uv = vec2( 0.f, 0.f );
    verts[2].uv = vec2( 1.f, 0.f );
    
    verts[0].norm = verts[1].norm = verts[2].norm = vec3( 0.f, 0.f, 1.f );
    
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
    
    resetDrawMode();
    
	return true;
}

/******************************************************************************
 *      LINES
******************************************************************************/
line::line( line&& l ) :
    primitive( std::move( l ) ),
    points{ l.points[0], l.points[1] }
{
    l.points[0] = vec3( 0.f );
    l.points[1] = vec3( 1.f );
}

line& line::operator = ( line&& l ) {
    primitive::operator=( std::move( l ) );
    points[0] = l.points[0];
    points[1] = l.points[1];
    l.points[0] = vec3( 0.f );
    l.points[1] = vec3( 1.f );
    return *this;
}

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
        GL_ARRAY_BUFFER, sizeof( points ),
        points[0].v, GL_STREAM_DRAW
    );
    printGlError( "Initializing a line primitive" );
    glVertexAttribPointer( pipeline::HGE_ATTRIB_VERTEX, 3, GL_FLOAT, GL_FALSE, 0, 0 );
    glEnableVertexAttribArray( pipeline::HGE_ATTRIB_VERTEX );
    
    glDisableVertexAttribArray( pipeline::HGE_ATTRIB_TEXTURE );
    glDisableVertexAttribArray( pipeline::HGE_ATTRIB_NORMAL );
    glDisableVertexAttribArray( pipeline::HGE_ATTRIB_TANGENT );
    
    glBindVertexArray( 0 );
    
    resetDrawMode();
    
    return true;
}

void line::setVertPos( int index, const vec3& inPos ) {
    HGE_ASSERT( (index >= 0) && (index < 2) );
    points[ index ] = inPos;
    
    glBindVertexArray( vao );
    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glBufferData( GL_ARRAY_BUFFER, sizeof( points ), &points[0].v, GL_STREAM_DRAW );
    glBindVertexArray( 0 );
}

void line::terminate() {
    primitive::terminate();
    points[0] = vec3( 0.f );
    points[1] = vec3( 1.f );
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
    hge::bumpVertex* verts = new bumpVertex[24];

    /*
     * POSITIONS
     */
    // back face
    verts[0].pos = vec3( -0.5f, -0.5f, -0.5f );
    verts[1].pos = vec3( -0.5f, 0.5f, -0.5f );
    verts[2].pos = vec3( 0.5f, -0.5f, -0.5f );
    verts[3].pos = vec3( 0.5f, 0.5f, -0.5f );

    // front face
    verts[4].pos = vec3( 0.5f, -0.5f, 0.5f );
    verts[5].pos = vec3( 0.5f, 0.5f, 0.5f );
    verts[6].pos = vec3( -0.5f, -0.5f, 0.5f );
    verts[7].pos = vec3( -0.5f, 0.5f, 0.5f );

    // right
    verts[8].pos = vec3( 0.5f, -0.5f, -0.5f );
    verts[9].pos = vec3( 0.5f, 0.5f, -0.5f );
    verts[10].pos = vec3( 0.5f, -0.5f, 0.5f );
    verts[11].pos = vec3( 0.5f, 0.5f, 0.5f );

    // left
    verts[12].pos = vec3( -0.5f, -0.5f, 0.5f );
    verts[13].pos = vec3( -0.5f, 0.5f, 0.5f );
    verts[14].pos = vec3( -0.5f, -0.5f, -0.5f );
    verts[15].pos = vec3( -0.5f, 0.5f, -0.5f );

    // bottom
    verts[16].pos = vec3( -0.5f, -0.5f, 0.5f );
    verts[17].pos = vec3( -0.5f, -0.5f, -0.5f );
    verts[18].pos = vec3( 0.5f, -0.5f, 0.5f );
    verts[19].pos = vec3( 0.5f, -0.5f, -0.5f );

    // top
    verts[20].pos = vec3( -0.5f, 0.5f, -0.5f );
    verts[21].pos = vec3( -0.5f, 0.5f, 0.5f );
    verts[22].pos = vec3( 0.5f, 0.5f, -0.5f );
    verts[23].pos = vec3( 0.5f, 0.5f, 0.5f );

    /*
     *  UV
     */
    verts[0].uv = vec2( 0.f, 0.f );
    verts[1].uv = vec2( 0.f, 1.f );
    verts[2].uv = vec2( 1.f, 0.f );
    verts[3].uv = vec2( 1.f, 1.f );

    verts[4].uv = vec2( 0.f, 0.f );
    verts[5].uv = vec2( 0.f, 1.f );
    verts[6].uv = vec2( 1.f, 0.f );
    verts[7].uv = vec2( 1.f, 1.f );

    verts[8].uv = vec2( 0.f, 0.f );
    verts[9].uv = vec2( 0.f, 1.f );
    verts[10].uv = vec2( 1.f, 0.f );
    verts[11].uv = vec2( 1.f, 1.f );

    verts[12].uv = vec2( 0.f, 0.f );
    verts[13].uv = vec2( 0.f, 1.f );
    verts[14].uv = vec2( 1.f, 0.f );
    verts[15].uv = vec2( 1.f, 1.f );

    verts[16].uv = vec2( 0.f, 0.f );
    verts[17].uv = vec2( 0.f, 1.f );
    verts[18].uv = vec2( 1.f, 0.f );
    verts[19].uv = vec2( 1.f, 1.f );

    verts[20].uv = vec2( 0.f, 0.f );
    verts[21].uv = vec2( 0.f, 1.f );
    verts[22].uv = vec2( 1.f, 0.f );
    verts[23].uv = vec2( 1.f, 1.f );

    /*
     * NORMALS
     */
    verts[0].norm =
        verts[1].norm =
            verts[2].norm =
                verts[3].norm = vec3( 0.f, 0.f, -1.f );

    verts[4].norm =
        verts[5].norm =
            verts[6].norm =
                verts[7].norm = vec3( 0.f, 0.f, 1.f );

    verts[8].norm =
        verts[9].norm =
            verts[10].norm =
                verts[11].norm = vec3( 1.f, 0.f, 0.f );

    verts[12].norm =
        verts[13].norm =
            verts[14].norm =
                verts[15].norm = vec3( -1.f, 0.f, 0.f );

    verts[16].norm =
        verts[17].norm =
            verts[18].norm =
                verts[19].norm = vec3( 0.f, -1.f, 0.f );

    verts[20].norm =
        verts[21].norm =
            verts[22].norm =
                verts[23].norm = vec3( 0.f, 1.f, 0.f );

    for ( unsigned i = 0; i < 24; i+=4 ) {
        calcTangents( verts[i+0], verts[i+1], verts[i+2] );
        calcTangents( verts[i+1], verts[i+2], verts[i+3] );
    }
    
    glBufferData( GL_ARRAY_BUFFER, sizeof( verts ), verts, GL_STATIC_DRAW );
    delete [] verts;
    verts = nullptr;
    
    pipeline::enableBumpVertexAttribs();
    
    glBindVertexArray( 0 );
    printGlError( "Creating a cube object" );
    
    resetDrawMode();
    
    return true;
}

/******************************************************************************
 *      SPHERES
******************************************************************************/
//
//bool sphere::init( int rings, int sectors ) {
//    float const R = 1.f / (float)(rings-1);
//    float const S = 1.f / (float)(sectors-1);
//    
//    if (vao)
//        terminate();
//    
//    unsigned numVerts = rings * sectors;
//    
//    bumpVertex* vertices = new( std::nothrow ) bumpVertex[ numVerts ];
//    if ( !vertices )
//        return false;
//        
//    GLuint* indices = new( std::nothrow ) GLuint[ numVerts * 4 ];
//    if ( !indices ) {
//        delete [] vertices;
//        vertices = nullptr;
//        return false;
//    }
//    
//    for( int r = 0, i = 0, j = 0; r < rings; ++r ) {
//        for( int s = 0; s < sectors; ++s ) {
//            const float y = std::sin( -HL_PI_OVER_2 + HL_PI * r * R );
//            const float x = std::cos( HL_TWO_PI * s * S ) * std::sin( HL_PI * r * R );
//            const float z = std::sin( HL_TWO_PI * s * S ) * std::sin( HL_PI * r * R );
//            
//            vertices[ i ].uv    = vec2( s*S, r*R );
//            vertices[ i ].norm  = // the normals are equal to the vertex position
//            vertices[ i ].pos   = vec3( x, y, z );
//            
//            const vec3 zenith   = vec3( 0.f, 1.f, 0.f );
//            const vec3 temp     = vertices[ i ].pos + zenith;
//            vertices[ i ].tng   = normalize( cross( zenith, temp ) );
//            vertices[ i ].btng  = normalize( cross( vertices[ i ].tng, zenith ) );
//            
//            ++i;
//            
//            indices[ j++ ] = r * sectors + s;
//            indices[ j++ ] = (r+1) * sectors + s;
//            indices[ j++ ] = r * sectors + (s+1);
//            indices[ j++ ] = (r+1) * sectors + (s+1);
//        }
//    }
//    
//	glGenVertexArrays( 1, &vao );
//	glGenBuffers( 1, &vbo );
//	glGenBuffers( 1, &ibo );
//    
//    if ( !vao || !vbo || !ibo ) {
//        std::cerr
//            << "An error occurred while initializing the sphere primitives"
//            << std::endl;
//        terminate();
//        return false;
//    }
//    
//    numIndices = numVerts * 4;
//    
//	glBindVertexArray( vao );
//	
//	glBindBuffer( GL_ARRAY_BUFFER, vbo );
//    glBufferData( GL_ARRAY_BUFFER, numVerts*sizeof(bumpVertex)-rings, vertices, GL_STATIC_DRAW );
//	printGlError( "Error while sending sphere primitive data to the GPU.");
//    
//    pipeline::enableBumpVertexAttribs();
//    
//    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibo );
//    glBufferData( GL_ELEMENT_ARRAY_BUFFER, numIndices*sizeof(GLuint)-numVerts, indices, GL_STATIC_DRAW );
//	
//	glBindVertexArray( 0 );
//    
//    delete [] vertices;
//    delete [] indices;
//    
//    resetDrawMode();
//    
//	return true;
//}

std::vector< bumpVertex > subdivide( std::vector< bumpVertex >& inVerts ) {
    std::vector< bumpVertex > newVerts( inVerts.size() * 4 );
    
    for (unsigned i = 0, j = 0; i < inVerts.size(); i+=3) {
        bumpVertex a;
        bumpVertex b;
        bumpVertex c;
        
        a.pos = ( inVerts[i+0].pos + inVerts[i+1].pos ) * 0.5f;
        b.pos = ( inVerts[i+1].pos + inVerts[i+2].pos ) * 0.5f;
        c.pos = ( inVerts[i+2].pos + inVerts[i+0].pos ) * 0.5f;
        /*
        a.norm = ( inVerts[i+0].norm + inVerts[i+1].norm ) * 0.5f;
        b.norm = ( inVerts[i+1].norm + inVerts[i+2].norm ) * 0.5f;
        c.norm = ( inVerts[i+2].norm + inVerts[i+0].norm ) * 0.5f;
        
        a.uv = ( inVerts[i+0].uv + inVerts[i+1].uv ) * 0.5f;
        b.uv = ( inVerts[i+1].uv + inVerts[i+2].uv ) * 0.5f;
        c.uv = ( inVerts[i+2].uv + inVerts[i+0].uv ) * 0.5f;
        
        a.tng = ( inVerts[i+0].tng + inVerts[i+1].tng ) * 0.5f;
        b.tng = ( inVerts[i+1].tng + inVerts[i+2].tng ) * 0.5f;
        c.tng = ( inVerts[i+2].tng + inVerts[i+0].tng ) * 0.5f;
        
        a.btng = ( inVerts[i+0].btng + inVerts[i+1].btng ) * 0.5f;
        b.btng = ( inVerts[i+1].btng + inVerts[i+2].btng ) * 0.5f;
        c.btng = ( inVerts[i+2].btng + inVerts[i+0].btng ) * 0.5f;
        */
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

bool sphere::init( int recursionDepth ) {
    if (vao)
        terminate();
    
    std::vector< bumpVertex > vertices( 60 );
    
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
        
        // Subdivide if necessary
        while( recursionDepth-- ) {
            vertices = std::move( subdivide( vertices ) );
        }
    }
    
    // Populate the UV/Normal/Tangent Data
    for ( unsigned i = 0; i < vertices.size(); ++i ) {
        bumpVertex& vert = vertices[i];
        
        // Add data to the normals
        vert.norm = vert.pos = normalize( vert.pos );
        
        vert.uv = vec2(
            0.5f + (std::atan2(vert.norm[0], vert.norm[2])/HL_TWO_PI),
            0.5 - (std::asin(vert.norm[1])/HL_PI)
        );
        
        // finally, do the tangents and bitangents
        if ( vert.norm[1] == 1.f ) {
            vert.tng            = vec3( 1.f, 0.f, 0.f );
            vert.btng           = vec3( 0.f, 0.f, 1.f );
        }
        else if ( vert.norm[1] == -1.f ) {
            vert.tng            = vec3( -1.f, 0.f, 0.f );
            vert.btng           = vec3( 0.f, 0.f, -1.f );
        }
        else {
            const vec3 zenith   = vec3( 0.f, 1.f, 0.f );
            const vec3 temp     = vert.pos;
            vert.tng            = normalize( cross( zenith, temp ) );
            vert.btng           = normalize( cross( vert.tng, zenith ) );
        }
    }
    
	glGenVertexArrays( 1, &vao );
	glGenBuffers( 1, &vbo );
    
    if ( !vao || !vbo ) {
        std::cerr
            << "An error occurred while initializing the sphere primitives"
            << std::endl;
        terminate();
        return false;
    }
    
    numTris = vertices.size();
    
	glBindVertexArray( vao );
	
	glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glBufferData( GL_ARRAY_BUFFER, sizeof(bumpVertex)*numTris, vertices.data(), GL_STATIC_DRAW );
	printGlError( "Error while sending sphere primitive data to the GPU.");
    
    pipeline::enableBumpVertexAttribs();
    
	glBindVertexArray( 0 );
    
    resetDrawMode();
    
	return true;
}

bool sphere::init() {
    return this->init( 0 );
}

void sphere::terminate() {
    glDeleteVertexArrays( 1, &vao );
    glDeleteBuffers( 1, &vbo );

    vao = vbo = 0;
    numTris = 0;
    
    resetDrawMode();
}

/******************************************************************************
 *      CONES
******************************************************************************/
cone::cone( cone&& c ) :
    primitive( std::move( c ) ),
    numVerts( c.numVerts )
{
    c.numVerts = 0;
}

cone& cone::operator = ( cone&& c ) {
    primitive::operator=( std::move( c ) );
    numVerts = c.numVerts;
    c.numVerts = 0;
    return *this;
}

bool cone::init( int sectors ) {
    // make sure there are enough points for a minimal pyramid
    if ( sectors < 3 ) {
        sectors = 3;
    }
    
    if ( vao && (numVerts == sectors+2 ) ) {
        return true;
    }
    else {
        terminate();
    }
    
    // add an offset of + 4 for the starting & ending vertices of the cone
    bumpVertex* baseVerts = new( std::nothrow ) bumpVertex[ (sectors*2) + 4 ];
    if ( !baseVerts ) {
        return false;
    }
    
    // add a +2 offset for the start and end vertices of the base of the cone
    bumpVertex* apexVerts = &baseVerts[ sectors+2 ];
    
    baseVerts[0].pos    = vec3( 0.f );
    baseVerts[0].uv     = vec2( 0.5f );
    baseVerts[0].norm   = vec3( 0.f, -1.f, 0.f );
    apexVerts[0].pos    = vec3( 0.f, 1.f, 0.f );
    apexVerts[0].uv     = vec2( 0.5f );
    apexVerts[0].norm   = vec3( 0.f, 1.f, 0.f );
    
    float apexAngle = 0.f;
    float baseAngle = 0.f;
    const float resolution = HL_TWO_PI / (float)sectors;
    
    for ( unsigned i = 0; i <= sectors; ++i ) {
        float bs = std::sin( baseAngle ) * 0.5f;
        float bc = std::cos( baseAngle ) * 0.5f;
        float as = std::sin( apexAngle ) * 0.5f;
        float ac = std::cos( apexAngle ) * 0.5f;
        
        bumpVertex& base = baseVerts[i+1];
        bumpVertex& apex = apexVerts[i+1];
        
        base.pos    = vec3( bs, 0.f, bc );
        base.uv     = vec2( bs+0.5f, bc+0.5f );
        base.norm   = vec3( 0.f, -1.f, 0.f );
        
        apex.pos    = vec3( as, 0.f, ac );
        apex.uv     = vec2( as+0.5f, ac+0.5f );
        apex.norm   = normalize( vec3( as, -std::sqrt( (ac*ac)+(as*as) ), ac ) );
        
        if ( i > 2 ) {
            calcTangents( baseVerts[i-0], baseVerts[i-1], baseVerts[i-2] );
            calcTangents( apexVerts[i-0], apexVerts[i-1], apexVerts[i-2] );
        }
        apexAngle += resolution;
        baseAngle -= resolution;
    }
    
    // Blend the first and last vertex tangents to fix a seam between them
    calcTangents( baseVerts[ sectors+1 ], baseVerts[0], baseVerts[1] );
    calcTangents( apexVerts[ sectors+1 ], apexVerts[0], apexVerts[1] );
    
    // The Tangents and Bitangents of the first, last, and apex vertices
    // don't contain the correct y-value
    apexVerts[ sectors+1 ].tng[1] = apexVerts[0].tng[1] = apexVerts[1].tng[1] = 0.f;
    apexVerts[ sectors+1 ].btng[1] = apexVerts[0].btng[1] = apexVerts[1].btng[1] = 0.f;
    
	glGenVertexArrays( 1, &vao );
	glBindVertexArray( vao );
	glGenBuffers( 1, &vbo );
    
    if ( !vao || !vbo ) {
        std::cerr
            << "An error occurred while initializing the cone primitives"
            << std::endl;
        terminate();
        return false;
    }
	
	glBindBuffer( GL_ARRAY_BUFFER, vbo );
    // add a +4 offset to "sectors" in order to account for the starting and
    // ending vertices of the base & apex of the cone
    glBufferData( GL_ARRAY_BUFFER, (sectors+4) * sizeof( bumpVertex ) * 2, baseVerts, GL_STATIC_DRAW );
	printGlError( "Error while sending sphere primitive data to the GPU.");
    
    pipeline::enableBumpVertexAttribs();
	
	glBindVertexArray( 0 );
    
    delete [] baseVerts;
    
    // again, account for the start and end vertices
    numVerts = sectors+2;
    resetDrawMode();
    
    return true;
}

void cone::terminate() {
    glDeleteVertexArrays( 1, &vao );
    glDeleteBuffers( 1, &vbo );

    vao = vbo = 0;
    numVerts = 0;
}

/******************************************************************************
 *      CIRCLES
******************************************************************************/
circle::circle( circle&& c ) :
    primitive( std::move( c ) ),
    numVerts( c.numVerts )
{
    c.numVerts = 0;
}

circle& circle::operator = ( circle&& c ) {
    primitive::operator=( std::move( c ) );
    numVerts = c.numVerts;
    c.numVerts = 0;
    return *this;
}

bool circle::init( int sectors ) {
    // make sure there are enough points for a minimal pyramid
    if ( sectors < 3 ) {
        sectors = 3;
    }
    
    if ( vao && (numVerts == sectors+2 ) ) {
        return true;
    }
    else {
        terminate();
    }
    
    // add an offset of + 4 for the starting & ending vertices of the circle
    bumpVertex* verts = new( std::nothrow ) bumpVertex[ sectors + 2 ];
    if ( !verts ) {
        return false;
    }
    
    verts[0].pos    = vec3( 0.f );
    verts[0].uv     = vec2( 0.5f );
    verts[0].norm   = vec3( 0.f, 0.f, 1.f );
    
    float baseAngle = 0.f;
    const float resolution = HL_TWO_PI / (float)sectors;
    
    for ( unsigned i = 0; i <= sectors; ++i ) {
        float bc = std::cos( baseAngle ) * 0.5f;
        float bs = std::sin( baseAngle ) * 0.5f;
        
        bumpVertex& currVert = verts[i+1];
        
        currVert.pos    = vec3( bs, bc, 0.f );
        currVert.uv     = vec2( bs+0.5f, bc+0.5f );
        currVert.norm   = vec3( 0.f, 0.f, 1.f );
        
        if ( i >= 2 ) {
            calcTangents( verts[i-0], verts[i-1], verts[i-2] );
        }
        baseAngle -= resolution;
    }
    // Blend the first and last vertex tangents to fix a seam between them
    calcTangents( verts[ sectors + 1], verts[0], verts[1] );
    
	glGenVertexArrays( 1, &vao );
	glBindVertexArray( vao );
	glGenBuffers( 1, &vbo );
    
    if ( !vao || !vbo ) {
        std::cerr
            << "An error occurred while initializing the circle primitives"
            << std::endl;
        terminate();
        return false;
    }
	
	glBindBuffer( GL_ARRAY_BUFFER, vbo );
    // add a +4 offset to "sectors" in order to account for the starting and
    // ending vertices of the base & apex of the circle
    glBufferData( GL_ARRAY_BUFFER, (sectors+2) * sizeof( bumpVertex ), verts, GL_STATIC_DRAW );
	printGlError( "Error while sending sphere primitive data to the GPU.");
    
    pipeline::enableBumpVertexAttribs();
	
	glBindVertexArray( 0 );
    
    delete [] verts;
    
    // again, account for the start and end vertices
    numVerts = sectors+2;
    resetDrawMode();
    
    return true;
}

void circle::terminate() {
    glDeleteVertexArrays( 1, &vao );
    glDeleteBuffers( 1, &vbo );

    vao = vbo = 0;
    numVerts = 0;
}

} // end hge namespace
