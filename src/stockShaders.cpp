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
    currPipeline( s.currPipeline ),
    program( std::move( s.program ) )
{
    s.currPipeline = nullptr;
}

stockShader& stockShader::operator = ( stockShader&& s ) {
    program = std::move( s.program );
    currPipeline = s.currPipeline;
    s.currPipeline = nullptr;
    
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

bool pointLightShader::init( pipeline* const p ) {
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
    GLint progId    = program.getProgramId();
    p->applyStockShader( progId );
    
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
    
    if (    ambColorId      == pipeline::INVALID_UNIFORM
    ||      ambIntId        == pipeline::INVALID_UNIFORM
    ||      pointColorId    == pipeline::INVALID_UNIFORM
    ||      pointIntId      == pipeline::INVALID_UNIFORM
    ||      pointConstId    == pipeline::INVALID_UNIFORM
    ||      pointLinearId   == pipeline::INVALID_UNIFORM
    ||      pointExpId      == pipeline::INVALID_UNIFORM
    ||      pointPosId      == pipeline::INVALID_UNIFORM
    ||      shadowId        == pipeline::INVALID_UNIFORM
    ||      lightMatrixId   == pipeline::INVALID_UNIFORM
    ||      textureId       == pipeline::INVALID_UNIFORM
    ||      normalId        == pipeline::INVALID_UNIFORM
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
    
    currPipeline = p;
    return true;
}

void pointLightShader::terminate() {
    currPipeline = nullptr;
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
bool shadowShader::init( pipeline* const p ) {
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
    
    GLint progId = program.getProgramId();
    p->applyStockShader( progId );
    GLint shadowMapId = program.getVariableId( "shadowMap" );
    
    printGlError("Shader setup error");
    
    if ( shadowMapId == pipeline::INVALID_UNIFORM ) {
        printGlError("Error accessing a shadowmap uniform variable");
        glUseProgram( 0 );
        return false;
    }
    
    glUniform1i( shadowMapId, pipeline::HGE_SAMPLER_SHADOWMAP );
    printGlError( "Shadowmap sampler error" );
    glUseProgram( 0 );
    
    currPipeline = p;
    return true;
}

/******************************************************************************
 * SKY SHADER
 ******************************************************************************/
bool skyShader::init( pipeline* const p ) {
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
        << "Setting up the Skybox shader.\nID: "
        << program.getProgramId()
        << std::endl;
    
    GLint progId = program.getProgramId();
    p->applyStockShader( progId );
    GLint skyTexId = program.getVariableId( "cubeTex" );
    printGlError("Skybox shader setup error");
    
    if ( skyTexId == pipeline::INVALID_UNIFORM ) {
        printGlError("Error accessing a skybox uniform variable");
        glUseProgram( 0 );
        return false;
    }
    
    glUniform1i( skyTexId, pipeline::HGE_SAMPLER_DIFFUSE );
    printGlError( "Skybox sampler error" );
    glUseProgram( 0 );
    
    currPipeline = p;
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

bool fontShader::init( pipeline* const p ) {
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
        << "Setting up the font shader.\nID: "
        << program.getProgramId()
        << std::endl;
    
    GLint progId = program.getProgramId();
    p->applyStockShader( progId );
    
    fontColId = program.getVariableId( "color" );
    GLint fontSampler = program.getVariableId( "texSampler" );
    printGlError("Font shader setup error");
    
    if (    fontSampler == pipeline::INVALID_UNIFORM
    ||      fontColId   == pipeline::INVALID_UNIFORM
    ) {
        printGlError("Error accessing a font uniform variable");
        glUseProgram( 0 );
        return false;
    }
    
    glUniform1i( fontSampler, pipeline::HGE_SAMPLER_DIFFUSE );
    printGlError( "Font Sampler error" );
    glUseProgram( 0 );
    
    currPipeline = p;
    return true;
}

void fontShader::terminate() {
    currPipeline = nullptr;
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

bool billboardShader::init( pipeline* const p ) {
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
        << "Setting up the billboard shader.\nID: "
        << program.getProgramId()
        << std::endl;
    
    GLint progId = program.getProgramId();
    p->applyStockShader( progId );
    
    camPosId = program.getVariableId( "camPos" );
    GLint texSampler = program.getVariableId( "texSampler" );
    printGlError("Shader setup error");
    
    if (    camPosId    == pipeline::INVALID_UNIFORM
    ||      texSampler== pipeline::INVALID_UNIFORM
    ) {
        printGlError("Error accessing a billboard uniform variable");
        glUseProgram( 0 );
        return false;
    }
    
    glUniform1i( texSampler, pipeline::HGE_SAMPLER_DEFAULT );
    
    printGlError( "Billboard Sampler error" );
    glUseProgram( 0 );
    
    currPipeline = p;
    return true;
}

void billboardShader::terminate() {
    currPipeline = nullptr;
    program.unload();
    camPosId = 0;
}

/******************************************************************************
 * NBT SHADER
 ******************************************************************************/
vnbtShader::vnbtShader( vnbtShader&& s ) :
    stockShader     ( std::move( s ) ),
    showVertId      ( s.showVertId ),
    showNormId      ( s.showNormId ),
    showTangId      ( s.showTangId ),
    showBtngId      ( s.showBtngId )
{
    s.showVertId    = 0;
    s.showNormId    = 0;
    s.showTangId    = 0;
    s.showBtngId    = 0;
}

vnbtShader& vnbtShader::operator = ( vnbtShader&& s ) {
    stockShader::operator=( std::move( s ) );
    showVertId     = s.showVertId;
    showNormId      = s.showNormId;
    showTangId      = s.showTangId;
    showBtngId      = s.showBtngId;
    
    s.showVertId    = 0;
    s.showNormId    = 0;
    s.showTangId    = 0;
    s.showBtngId    = 0;
    return *this;
}

bool vnbtShader::init( pipeline* const p ) {
    // don't do anything if there already is a shader program in memory
    if ( program.getProgramId() )
        return true;
    
    if (    !program.loadBuffer( ntbVisualizerVS, sizeof( ntbVisualizerVS ), GL_VERTEX_SHADER )
    ||      !program.loadBuffer( ntbVisualizerGS, sizeof( ntbVisualizerGS ), GL_GEOMETRY_SHADER )
    ||      !program.loadBuffer( ntbVisualizerFS, sizeof( ntbVisualizerFS ), GL_FRAGMENT_SHADER )
    ||      !program.compile()
    ) {
        return false;
    }
    
    std::cout
        << "Setting up the Normal/Tangent/Billboard visualization shader.\nID: "
        << program.getProgramId()
        << std::endl;
    
    GLint progId = program.getProgramId();
    p->applyStockShader( progId );
    
    showVertId = program.getVariableId( "showVertices" );
    showNormId = program.getVariableId( "showNormals" );
    showTangId = program.getVariableId( "showTangents" );
    showBtngId = program.getVariableId( "showBitangents" );
    printGlError("Shader setup error");
    
    if (    showVertId == pipeline::INVALID_UNIFORM
    ||      showNormId == pipeline::INVALID_UNIFORM
    ||      showTangId == pipeline::INVALID_UNIFORM
    ||      showBtngId == pipeline::INVALID_UNIFORM
    ) {
        printGlError("Error accessing an NBT uniform variable");
        return false;
        glUseProgram( 0 );
    }
    
    glUseProgram( 0 );
    
    currPipeline = p;
    return true;
}

void vnbtShader::terminate() {
    currPipeline = nullptr;
    program.unload();
    showVertId = 0;
    showNormId = 0;
    showTangId = 0;
    showBtngId = 0;
}

} // end hge namespace
