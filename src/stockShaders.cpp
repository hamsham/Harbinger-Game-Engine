/* 
 * File:   shaderMgr.cpp
 * Author: hammy
 * 
 * Created on May 14, 2013, 12:25 AM
 */

#include <iostream>
#include <utility>

#include "light.h"
#include "pipeline.h"
#include "shader.h"
#include "stockShaders.h"
#include "stockShaders_glsl.h"

namespace hge {
    
stockShader::stockShader( stockShader&& s ) :
    program( std::move( s.program ) )
{}

stockShader& stockShader::operator = ( stockShader&& s ) {
    program = std::move( s.program );
    return *this;
}

/******************************************************************************
 * POINT LIGHT SHADER
 ******************************************************************************/
pointLightShader::pointLightShader( pointLightShader&& p ) :
    stockShader( std::move( p ) ),
    ambColorId( p.ambColorId ),
    ambIntId( p.ambIntId ),
    pointColorId( p.pointColorId ),
    pointIntId( p.pointIntId ),
    pointLinearId( p.pointLinearId ),
    pointConstId( p.pointConstId ),
    pointExpId( p.pointExpId ),
    pointPosId( p.pointPosId ),
    lightMatrixId( p.lightMatrixId )
{
    p.ambColorId      = 0;
    p.ambIntId        = 0;
    p.pointColorId    = 0;
    p.pointIntId      = 0;
    p.pointLinearId   = 0;
    p.pointConstId    = 0;
    p.pointExpId      = 0;
    p.pointPosId      = 0;
    p.lightMatrixId   = 0;
}

pointLightShader& pointLightShader::operator = ( pointLightShader&& p ) {
    stockShader::operator=( std::move( p ) );
    ambColorId      = p.ambColorId;
    ambIntId        = p.ambIntId;
    pointColorId    = p.pointColorId;
    pointIntId      = p.pointIntId;
    pointLinearId   = p.pointLinearId;
    pointConstId    = p.pointConstId;
    pointExpId      = p.pointExpId;
    pointPosId      = p.pointPosId;
    lightMatrixId   = p.lightMatrixId;
    
    p.ambColorId      = 0;
    p.ambIntId        = 0;
    p.pointColorId    = 0;
    p.pointIntId      = 0;
    p.pointLinearId   = 0;
    p.pointConstId    = 0;
    p.pointExpId      = 0;
    p.pointPosId      = 0;
    p.lightMatrixId   = 0;
    return *this;
}

bool pointLightShader::init() {
    // don't do anything if there already is a shader program in memory
    if ( program.getProgramId() )
        return true;
    
    std::cout << "Loading a point light shader." << std::endl;
    if (    !program.loadBuffer( pointVS, sizeof( pointVS ), GL_VERTEX_SHADER )
    ||      !program.loadBuffer( pointFS, sizeof( pointFS ), GL_FRAGMENT_SHADER )
    ||      !program.compile()
    ) {
        std::cerr << "Failed to load a point light shader." << std::endl;
        return false;
    }
    
    std::cout
        << "Setting up a point light shader.\n\tID: "
        << program.getProgramId()
        << std::endl;
    
    GLint textureId = 0;
    GLint shadowId  = 0;
    GLint normalId  = 0;
    glUseProgram( program.getProgramId() );
    
    ambColorId      = program.getVariableId( "ambientLight.color" );
    ambIntId        = program.getVariableId( "ambientLight.intensity" );
    pointColorId    = program.getVariableId( "pointLight.color" );
    pointIntId      = program.getVariableId( "pointLight.intensity" );
    pointConstId    = program.getVariableId( "pointLight.constant" );
    pointLinearId   = program.getVariableId( "pointLight.linear" );
    pointExpId      = program.getVariableId( "pointLight.exponential" );
    pointPosId      = program.getVariableId( "pointLight.position" );
    shadowId        = program.getVariableId( "shadowMap" );
    lightMatrixId   = program.getVariableId( "lightMVP" );
    textureId       = program.getVariableId( "texSampler" );
    normalId        = program.getVariableId( "normalMap" );
    printGlError("Point light shader setup error");
    
    if (    ambColorId      == pipeline::HGE_ATTRIB_INVALID
    ||      ambIntId        == pipeline::HGE_ATTRIB_INVALID
    ||      pointColorId    == pipeline::HGE_ATTRIB_INVALID
    ||      pointIntId      == pipeline::HGE_ATTRIB_INVALID
    ||      pointConstId    == pipeline::HGE_ATTRIB_INVALID
    ||      pointLinearId   == pipeline::HGE_ATTRIB_INVALID
    ||      pointExpId      == pipeline::HGE_ATTRIB_INVALID
    ||      pointPosId      == pipeline::HGE_ATTRIB_INVALID
    ||      shadowId        == pipeline::HGE_ATTRIB_INVALID
    ||      lightMatrixId   == pipeline::HGE_ATTRIB_INVALID
    ||      textureId       == pipeline::HGE_ATTRIB_INVALID
    ||      normalId        == pipeline::HGE_ATTRIB_INVALID
    ) {
        printGlError("Error accessing a point light uniform variable");
        glUseProgram( 0 );
        return false;
    }
    
    glUniform1i( textureId, pipeline::HGE_SAMPLER_DIFFUSE );
    printGlError( "Texture Sampler error" );
    glUniform1i( normalId, pipeline::HGE_SAMPLER_NORMAL );
    printGlError( "Normal Sampler error" );
    glUniform1i( shadowId, pipeline::HGE_SAMPLER_SHADOWMAP );
    printGlError( "Shadowmap Sampler error" );
    glUseProgram( 0 );
    
    return true;
}

void pointLightShader::terminate() {
    program.unload();
    
    ambColorId      = 0;
    ambIntId        = 0;
    pointColorId    = 0;
    pointIntId      = 0;
    pointLinearId   = 0;
    pointConstId    = 0;
    pointExpId      = 0;
    pointPosId      = 0;
    lightMatrixId   = 0;
}

void pointLightShader::setLightMvp( const mat4& mvp ) {
    glUniformMatrix4fv( lightMatrixId, 1, GL_FALSE, (GLfloat*)mvp.m );
}

void pointLightShader::setPointLight( const pointLight& p ) {
    glUniform1f(    pointIntId,     p.intensity );
    glUniform1f(    pointLinearId,  p.linear );
    glUniform1f(    pointConstId,   p.constant );
    glUniform1f(    pointExpId,     p.exponential );
    glUniform4fv(   pointColorId,   1, p.color.v );
    glUniform3fv(   pointPosId,     1, p.pos.v );
}

void pointLightShader::setAmbientLight( const ambientLight& a ) {
    glUniform1f(    ambIntId,       a.intensity );
    glUniform4fv(   ambColorId,     1, a.color.v );
}

/******************************************************************************
 * SHADOW SHADER
 ******************************************************************************/
bool shadowShader::init() {
    // don't do anything if there already is a shader program in memory
    if ( program.getProgramId() )
        return true;
    
    if (    !program.loadBuffer( plainVS, sizeof( plainVS ), GL_VERTEX_SHADER )
    ||      !program.loadBuffer( shadowFS, sizeof( shadowFS ), GL_FRAGMENT_SHADER )
    ||      !program.compile()
    ) {
        return false;
    }
    std::cout
        << "Setting up a shadow map shader.\n\tID: "
        << program.getProgramId()
        << std::endl;
    
    glUseProgram( program.getProgramId() );
    GLint shadowMapId = program.getVariableId( "shadowMap" );
    
    printGlError("Shader setup error");
    
    if ( shadowMapId == pipeline::HGE_ATTRIB_INVALID ) {
        printGlError("Error accessing a shadowmap uniform variable");
        glUseProgram( 0 );
        return false;
    }
    
    glUniform1i( shadowMapId, pipeline::HGE_SAMPLER_SHADOWMAP );
    printGlError( "Shadowmap sampler error" );
    glUseProgram( 0 );
    
    return true;
}

/******************************************************************************
 * SKY SHADER
 ******************************************************************************/
bool skyShader::init() {
    // don't do anything if there already is a shader program in memory
    if ( program.getProgramId() )
        return true;
    
    if (    !program.loadBuffer( skyVS, sizeof( skyVS ), GL_VERTEX_SHADER )
    ||      !program.loadBuffer( skyFS, sizeof( skyFS ), GL_FRAGMENT_SHADER )
    ||      !program.compile()
    ) {
        return false;
    }
    
    std::cout
        << "Setting up the Skybox shader.\n\tID: "
        << program.getProgramId()
        << std::endl;
    
    glUseProgram( program.getProgramId() );
    GLint skyTexId = program.getVariableId( "cubeTex" );
    printGlError("Skybox shader setup error");
    
    if ( skyTexId == pipeline::HGE_ATTRIB_INVALID ) {
        printGlError("Error accessing a skybox uniform variable");
        glUseProgram( 0 );
        return false;
    }
    
    glUniform1i( skyTexId, pipeline::HGE_SAMPLER_DIFFUSE );
    printGlError( "Skybox sampler error" );
    glUseProgram( 0 );
    
    return true;
}

/******************************************************************************
 * FONT SHADER
 ******************************************************************************/
fontShader::fontShader( fontShader&& f ) :
    stockShader( std::move( f ) ),
    fontColId( f.fontColId )
{
    f.fontColId = 0;
}

fontShader& fontShader::operator = ( fontShader&& f ) {
    stockShader::operator=( std::move( f ) );
    fontColId = f.fontColId;
    f.fontColId = 0;
    return *this;
}

bool fontShader::init() {
    // don't do anything if there already is a shader program in memory
    if ( program.getProgramId() )
        return true;
    
    if (    !program.loadBuffer( plainVS, sizeof( plainVS ), GL_VERTEX_SHADER )
    ||      !program.loadBuffer( fontFS, sizeof( fontFS ), GL_FRAGMENT_SHADER )
    ||      !program.compile()
    ) {
        return false;
    }
    
    std::cout
        << "Setting up the font shader.\n\tID: "
        << program.getProgramId()
        << std::endl;
    
    glUseProgram( program.getProgramId() );
    
    fontColId = program.getVariableId( "color" );
    GLint fontSampler = program.getVariableId( "texSampler" );
    printGlError("Font shader setup error");
    
    if (    fontSampler == pipeline::HGE_ATTRIB_INVALID
    ||      fontColId   == pipeline::HGE_ATTRIB_INVALID
    ) {
        printGlError("Error accessing a font uniform variable");
        glUseProgram( 0 );
        return false;
    }
    
    glUniform1i( fontSampler, pipeline::HGE_SAMPLER_DIFFUSE );
    printGlError( "Font Sampler error" );
    glUseProgram( 0 );
    
    return true;
}

void fontShader::terminate() {
    program.unload();
    fontColId = 0;
}

/******************************************************************************
 * BILLBOARD SHADER
 ******************************************************************************/
billboardShader::billboardShader( billboardShader&& b ) :
    stockShader( std::move( b ) ),
    camPosId( b.camPosId )
{
    b.camPosId = 0;
}

billboardShader& billboardShader::operator = ( billboardShader&& b ) {
    stockShader::operator=( std::move( b ) );
    camPosId = b.camPosId;
    b.camPosId = 0;
    return *this;
}

bool billboardShader::init() {
    // don't do anything if there already is a shader program in memory
    if ( program.getProgramId() )
        return true;
    
    if (    !program.loadBuffer( billboardVS, sizeof( billboardVS ), GL_VERTEX_SHADER )
    ||      !program.loadBuffer( billboardGS, sizeof( billboardGS ), GL_GEOMETRY_SHADER )
    ||      !program.loadBuffer( billboardFS, sizeof( billboardFS ), GL_FRAGMENT_SHADER )
    ||      !program.compile()
    ) {
        return false;
    }
    
    std::cout
        << "Setting up the billboard shader.\n\tID: "
        << program.getProgramId()
        << std::endl;
    
    glUseProgram( program.getProgramId() );
    
    camPosId = program.getVariableId( "camPos" );
    GLint texSampler = program.getVariableId( "texSampler" );
    printGlError("Shader setup error");
    
    if (    camPosId    == pipeline::HGE_ATTRIB_INVALID
    ||      texSampler== pipeline::HGE_ATTRIB_INVALID
    ) {
        printGlError("Error accessing a billboard uniform variable");
        glUseProgram( 0 );
        return false;
    }
    
    glUniform1i( texSampler, pipeline::HGE_SAMPLER_DEFAULT );
    
    printGlError( "Billboard Sampler error" );
    glUseProgram( 0 );
    
    return true;
}

void billboardShader::terminate() {
    program.unload();
    camPosId = 0;
}

/******************************************************************************
 * NBT SHADER
 ******************************************************************************/
enbtShader::enbtShader( enbtShader&& s ) :
    stockShader     ( std::move( s ) ),
    showEdgeId      ( s.showEdgeId ),
    showNormId      ( s.showNormId ),
    showTangId      ( s.showTangId ),
    showBtngId      ( s.showBtngId )
{
    s.showEdgeId    = 0;
    s.showNormId    = 0;
    s.showTangId    = 0;
    s.showBtngId    = 0;
}

enbtShader& enbtShader::operator = ( enbtShader&& s ) {
    stockShader::operator=( std::move( s ) );
    showEdgeId     = s.showEdgeId;
    showNormId      = s.showNormId;
    showTangId      = s.showTangId;
    showBtngId      = s.showBtngId;
    
    s.showEdgeId    = 0;
    s.showNormId    = 0;
    s.showTangId    = 0;
    s.showBtngId    = 0;
    return *this;
}

bool enbtShader::init() {
    // don't do anything if there already is a shader program in memory
    if ( program.getProgramId() )
        return true;
    
    if (    !program.loadBuffer( enbtVS, sizeof( enbtVS ), GL_VERTEX_SHADER )
    ||      !program.loadBuffer( enbtGS, sizeof( enbtGS ), GL_GEOMETRY_SHADER )
    ||      !program.loadBuffer( enbtFS, sizeof( enbtFS ), GL_FRAGMENT_SHADER )
    ||      !program.compile()
    ) {
        return false;
    }
    
    std::cout
        << "Setting up the Normal/Tangent/Billboard visualization shader.\n\tID: "
        << program.getProgramId()
        << std::endl;
    
    glUseProgram( program.getProgramId() );
    
    showEdgeId = program.getVariableId( "showEdges" );
    showNormId = program.getVariableId( "showNormals" );
    showTangId = program.getVariableId( "showTangents" );
    showBtngId = program.getVariableId( "showBitangents" );
    printGlError("Shader setup error");
    
    if (    showEdgeId == pipeline::HGE_ATTRIB_INVALID
    ||      showNormId == pipeline::HGE_ATTRIB_INVALID
    ||      showTangId == pipeline::HGE_ATTRIB_INVALID
    ||      showBtngId == pipeline::HGE_ATTRIB_INVALID
    ) {
        printGlError("Error accessing an NBT uniform variable");
        return false;
        glUseProgram( 0 );
    }
    
    glUseProgram( 0 );
    
    return true;
}

void enbtShader::terminate() {
    program.unload();
    showEdgeId = 0;
    showNormId = 0;
    showTangId = 0;
    showBtngId = 0;
}

} // end hge namespace
