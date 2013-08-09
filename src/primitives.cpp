/* 
 * File:   primitives.cpp
 * Author: hammy
 * 
 * Created on April 22, 2013, 8:40 PM
 */

#include <iostream>
#include <utility>
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
    
    verts[0].norm = verts[1].norm = verts[2].norm = vec3( 0.f, 0.f, -1.f );
    
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
    glVertexAttribPointer( pipeline::VERTEX_ATTRIB, 3, GL_FLOAT, GL_FALSE, 0, 0 );
    glEnableVertexAttribArray( pipeline::VERTEX_ATTRIB );
    
    glDisableVertexAttribArray( pipeline::TEXTURE_ATTRIB );
    glDisableVertexAttribArray( pipeline::NORMAL_ATTRIB );
    glDisableVertexAttribArray( pipeline::TANGENT_ATTRIB );
    
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
    hge::bumpVertex verts[24];

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
    
    pipeline::enableBumpVertexAttribs();
    
    glBindVertexArray( 0 );
    printGlError( "Creating a cube object" );
    
    resetDrawMode();
    
    return true;
}

/******************************************************************************
 *      SPHERES
******************************************************************************/
sphere::sphere( sphere&& s ) :
    primitive( std::move( s ) ),
    ibo( s.ibo )
{
    s.ibo = 0;
}

sphere& sphere::operator = ( sphere&& s ) {
    primitive::operator=( std::move( s ) );
    ibo = s.ibo;
    s.ibo = 0;
    return *this;
}

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
    
    for( int r = 0, i = 0, j = 0; r < rings; r++ ) {
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
            
            if ( i >= 2 )
                calcTangents( vertices[i-0], vertices[i-1], vertices[i-2] );
            
            indices[ j++ ] = r * sectors + s;
            indices[ j++ ] = (r+1) * sectors + s;
            indices[ j++ ] = r * sectors + (s+1);
            indices[ j++ ] = (r+1) * sectors + (s+1);
        }
    }
    
	glGenVertexArrays( 1, &vao );
	glGenBuffers( 1, &vbo );
	glGenBuffers( 1, &ibo );
    
    if ( !vao || !vbo || !ibo ) {
        std::cerr
            << "An error occurred while initializing the sphere primitives"
            << std::endl;
        terminate();
        return false;
    }
    
    numIndices = numVerts * 4;
    
	glBindVertexArray( vao );
	
	glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glBufferData( GL_ARRAY_BUFFER, numVerts * sizeof( bumpVertex ), vertices, GL_STATIC_DRAW );
	printGlError( "Error while sending sphere primitive data to the GPU.");
    
    pipeline::enableBumpVertexAttribs();
    
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibo );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof( GLuint ), indices, GL_STATIC_DRAW );
	
	glBindVertexArray( 0 );
    
    delete [] vertices;
    delete [] indices;
    
    resetDrawMode();
    
	return true;
}

void sphere::terminate() {
    glDeleteVertexArrays( 1, &vao );
    glDeleteBuffers( 1, &vbo );
    glDeleteBuffers( 1, &ibo );

    vao = vbo = ibo = 0;
    numIndices = 0;
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
        
        if ( i >= 2 ) {
            calcTangents( baseVerts[i-0], baseVerts[i-1], baseVerts[i-2] );
            calcTangents( apexVerts[i-0], apexVerts[i-1], apexVerts[i-2] );
        }
        apexAngle += resolution;
        baseAngle -= resolution;
    }
    // it took a lot of trial and error to figure this out
    apexVerts[0].tng[1] = 0.f;
    apexVerts[0].btng[1] = 0.f;
    
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
void cone::draw() const {
    const GLint     first[] = {0,numVerts};
    const GLsizei   count[] = {numVerts, numVerts};
    
    glBindVertexArray( vao );
    glMultiDrawArrays( renderMode, first, count, 2 );
    glBindVertexArray( 0 );
}

} // end hge namespace
