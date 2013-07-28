/* 
 * File:   shaderMgr.cpp
 * Author: hammy
 * 
 * Created on May 14, 2013, 12:25 AM
 */

#include <iostream>

#include "camera.h"
#include "light.h"
#include "pipeline.h"
#include "shader.h"
#include "stockShaders.h"
#include "stockShaders_glsl.h"
#include "transformations.h"

namespace pipeline = hge::pipeline;
using hamLibs::math::mat4;
namespace hge {

void stockShaders::updateMatricesImpl() {
    // Upload the matrix data to the current shader
    glUniformBlockBinding( currShader, matrixIndexId, HGE_PIPELINE_MATRIX_BINDING );
    glBindBuffer( GL_UNIFORM_BUFFER, ubo );
    glBindBufferBase( GL_UNIFORM_BUFFER, HGE_PIPELINE_MATRIX_BINDING, ubo );

    glBufferData(
        GL_UNIFORM_BUFFER, sizeof( transforms ), transforms, GL_DYNAMIC_DRAW
    );
}

/******************************************************************************
 * Shader Manager Private Functions
 ******************************************************************************/

/******************************************************************************
 * POINT LIGHT SHADER
 ******************************************************************************/
bool stockShaders::initPointLightShader() {
    std::cout
        << "Setting up the point light shader.\nID: "
        << pointLightShader.getProgramId()
        << std::endl;
    
    hge::stockShaders::applyShader( pointLightShader.getProgramId() );
    
    ambColorId      = pointLightShader.getVariableId( "ambientLight.color" );
    ambIntId        = pointLightShader.getVariableId( "ambientLight.intensity" );
    pointColorId    = pointLightShader.getVariableId( "pointLight.color" );
    pointIntId      = pointLightShader.getVariableId( "pointLight.intensity" );
    pointConstId    = pointLightShader.getVariableId( "pointLight.constant" );
    pointLinearId   = pointLightShader.getVariableId( "pointLight.linear" );
    pointExpId      = pointLightShader.getVariableId( "pointLight.exponential" );
    pointPosId      = pointLightShader.getVariableId( "pointLight.position" );
    shadowId        = pointLightShader.getVariableId( "shadowMap" );
    lightMatrixId   = pointLightShader.getVariableId( "lightMVP" );
    textureId       = pointLightShader.getVariableId( "texSampler" );
    normalId        = pointLightShader.getVariableId( "normalMap" );
    printGlError("Point light shader setup error");
    
    if (
        ambColorId      == pipeline::INVALID_UNIFORM
    ||  ambIntId        == pipeline::INVALID_UNIFORM
    ||  pointColorId    == pipeline::INVALID_UNIFORM
    ||  pointIntId      == pipeline::INVALID_UNIFORM
    ||  pointConstId    == pipeline::INVALID_UNIFORM
    ||  pointLinearId   == pipeline::INVALID_UNIFORM
    ||  pointExpId      == pipeline::INVALID_UNIFORM
    ||  pointPosId      == pipeline::INVALID_UNIFORM
    ||  shadowId        == pipeline::INVALID_UNIFORM
    ||  lightMatrixId   == pipeline::INVALID_UNIFORM
    ||  textureId       == pipeline::INVALID_UNIFORM
    ||  normalId        == pipeline::INVALID_UNIFORM
    ||  camPosId        == pipeline::INVALID_UNIFORM
    ) {
        printGlError("Error accessing a point light uniform variable");
        return false;
    }
    
    glUniform1i( textureId, pipeline::HGE_SAMPLER_DIFFUSE );
    printGlError( "Texture Sampler error" );
    glUniform1i( normalId, pipeline::HGE_SAMPLER_NORMAL );
    printGlError( "Normal Sampler error" );
    glUniform1i( shadowId, pipeline::HGE_SAMPLER_SHADOWMAP );
    printGlError( "Shadowmap Sampler error" );
    
    return true;
}

/******************************************************************************
 * SHADOW SHADER
 ******************************************************************************/
bool stockShaders::initShadowShader() {
    std::cout
        << "Setting up the shadowmap shader.\nID: "
        << shadowShader.getProgramId()
        << std::endl;
    
    hge::stockShaders::applyShader( shadowShader.getProgramId() );
    
    shadowMapId = shadowShader.getVariableId( "shadowMap" );
    printGlError("Shader setup error");
    
    if ( shadowMapId == pipeline::INVALID_UNIFORM ) {
        printGlError("Error accessing a shadowmap uniform variable");
        return false;
    }
    
    glUniform1i( shadowMapId, pipeline::HGE_SAMPLER_SHADOWMAP );
    printGlError( "Shadowmap sampler error" );
    
    return true;
}

/******************************************************************************
 * SKY SHADER
 ******************************************************************************/
bool stockShaders::initSkyShader() {
    std::cout
        << "Setting up the Skybox shader.\nID: "
        << skyShader.getProgramId()
        << std::endl;
    
    hge::stockShaders::applyShader( skyShader.getProgramId() );
    
    skyTexId = skyShader.getVariableId( "cubeTex" );
    printGlError("Skybox shader setup error");
    
    if ( skyTexId == pipeline::INVALID_UNIFORM ) {
        printGlError("Error accessing a skybox uniform variable");
        return false;
    }
    
    glUniform1i( skyTexId, pipeline::HGE_SAMPLER_DIFFUSE );
    printGlError( "Skybox sampler error" );
    
    return true;
}

/******************************************************************************
 * FONT SHADER
 ******************************************************************************/
bool stockShaders::initFontShader() {
    std::cout
        << "Setting up the font shader.\nID: "
        << fontShader.getProgramId()
        << std::endl;
    
    hge::stockShaders::applyShader( fontShader.getProgramId() );
    
    fontColId   = fontShader.getVariableId( "color" );
    fontSampler = fontShader.getVariableId( "texSampler" );
    printGlError("Font shader setup error");
    
    if (    fontSampler == pipeline::INVALID_UNIFORM
    ||      fontColId   == pipeline::INVALID_UNIFORM
    ) {
        printGlError("Error accessing a font uniform variable");
        return false;
    }
    
    glUniform1i( fontSampler, pipeline::HGE_SAMPLER_DIFFUSE );
    printGlError( "Font Sampler error" );
    
    return true;
}

/******************************************************************************
 * BILLBOARD SHADER
 ******************************************************************************/
bool stockShaders::initBillboardShader() {
    std::cout
        << "Setting up the billboard shader.\nID: "
        << bbShader.getProgramId()
        << std::endl;
    
    hge::stockShaders::applyShader( bbShader.getProgramId() );
    
    camPosId        = bbShader.getVariableId( "camPos" );
    bbTexSampler    = bbShader.getVariableId( "texSampler" );
    printGlError("Shader setup error");
    
    if (    camPosId    == pipeline::INVALID_UNIFORM
    ||      bbTexSampler== pipeline::INVALID_UNIFORM
    ) {
        printGlError("Error accessing a billboard uniform variable");
        return false;
    }
    
    glUniform1i( bbTexSampler, pipeline::HGE_SAMPLER_DEFAULT );
    
    printGlError( "Billboard Sampler error" );
    
    return true;
}

/******************************************************************************
 * NBT SHADER
 ******************************************************************************/
bool stockShaders::initNbtShader() {
    std::cout
        << "Setting up the Normal/Tangent/Billboard visualization shader.\nID: "
        << nbtShader.getProgramId()
        << std::endl;
    
    hge::stockShaders::applyShader( nbtShader.getProgramId() );
    
    nbtShowNormId = nbtShader.getVariableId( "showNormals" );
    nbtShowTangId = nbtShader.getVariableId( "showTangents" );
    nbtShowBtngId = nbtShader.getVariableId( "showBitangents" );
    printGlError("Shader setup error");
    
    if (    nbtShowNormId == pipeline::INVALID_UNIFORM
    ||      nbtShowTangId == pipeline::INVALID_UNIFORM
    ||      nbtShowBtngId == pipeline::INVALID_UNIFORM
    ) {
        printGlError("Error accessing an NBT uniform variable");
        return false;
    }
    
    return true;
}

/******************************************************************************
 * Shader Manager Initialization
 ******************************************************************************/
stockShaders::stockShaders() {
    glGenBuffers( 1, &ubo );
    printGlError( "Creating pipeline uniform block" );

    HL_ASSERT( ubo != 0 );
    
    HL_ASSERT( shadowShader.loadBuffer( plainVS, sizeof( plainVS ), GL_VERTEX_SHADER ) );
    HL_ASSERT( shadowShader.loadBuffer( shadowFS, sizeof( shadowFS ), GL_FRAGMENT_SHADER ) );
    HL_ASSERT( shadowShader.compile() );
            
    HL_ASSERT( pointLightShader.loadBuffer( pointVS, sizeof( pointVS ), GL_VERTEX_SHADER ) );
    HL_ASSERT( pointLightShader.loadBuffer( pointFS, sizeof( pointFS ), GL_FRAGMENT_SHADER ) );
    HL_ASSERT( pointLightShader.compile() );
            
    HL_ASSERT( skyShader.loadBuffer( skyVS, sizeof( skyVS ), GL_VERTEX_SHADER ) );
    HL_ASSERT( skyShader.loadBuffer( skyFS, sizeof( skyFS ), GL_FRAGMENT_SHADER ) );
    HL_ASSERT( skyShader.compile() );
            
    HL_ASSERT( fontShader.loadBuffer( plainVS, sizeof( plainVS ), GL_VERTEX_SHADER ) );
    HL_ASSERT( fontShader.loadBuffer( fontFS, sizeof( fontFS ), GL_FRAGMENT_SHADER ) );
    HL_ASSERT( fontShader.compile() );
            
    HL_ASSERT( bbShader.loadBuffer( billboardVS, sizeof( billboardVS ), GL_VERTEX_SHADER ) );
    HL_ASSERT( bbShader.loadBuffer( billboardGS, sizeof( billboardGS ), GL_GEOMETRY_SHADER ) );
    HL_ASSERT( bbShader.loadBuffer( billboardFS, sizeof( billboardFS ), GL_FRAGMENT_SHADER ) );
    HL_ASSERT( bbShader.compile() );
    
    HL_ASSERT( nbtShader.loadBuffer( ntbVisualizerVS, sizeof( ntbVisualizerVS ),GL_VERTEX_SHADER ) );
    HL_ASSERT( nbtShader.loadBuffer( ntbVisualizerGS, sizeof( ntbVisualizerGS ),GL_GEOMETRY_SHADER ) );
    HL_ASSERT( nbtShader.loadBuffer( ntbVisualizerFS, sizeof( ntbVisualizerFS ),GL_FRAGMENT_SHADER ) );
    HL_ASSERT( nbtShader.compile() );
    
    printGlError("Shader compilation error");
    
    HL_ASSERT( initPointLightShader() );
    HL_ASSERT( initShadowShader() );
    HL_ASSERT( initSkyShader() );
    HL_ASSERT( initFontShader() );
    HL_ASSERT( initBillboardShader() );
    HL_ASSERT( initNbtShader() );
}

/******************************************************************************
 * Shader Manager Termination
 ******************************************************************************/
stockShaders::~stockShaders() {
    glDeleteBuffers( 1, &ubo );
    ubo = currShader = 0;
    matrixIndexId = GL_INVALID_INDEX;
    
    pointLightShader.unload();
    shadowShader.unload();
    fontShader.unload();
    skyShader.unload();
    bbShader.unload();
    nbtShader.unload();
    
    ambColorId      = 0;
    ambIntId        = 0;
    pointColorId    = 0;
    pointIntId      = 0;
    pointLinearId   = 0;
    pointConstId    = 0;
    pointExpId      = 0;
    pointPosId      = 0;
    shadowId        = 0;
    lightMatrixId   = 0;
    textureId       = 0;
    normalId        = 0;
    
    shadowMapId     = 0;
    
    skyTexId        = 0;
    
    fontColId       = 0;
    fontSampler     = 0;
    
    camPosId        = 0;
    
    nbtShowNormId   = 0;
    nbtShowTangId   = 0;
    nbtShowBtngId   = 0;
}

/******************************************************************************
 * Shader Management
******************************************************************************/
void stockShaders::applyShader( GLuint shaderId ) {
    currShader = shaderId;
    
    if ( !currShader )
        return;
    
    glUseProgram( currShader );
    printGlError( "Applying a shader to the pipeline" );
    
    glBindBuffer( GL_UNIFORM_BUFFER, ubo );
    glBindBufferBase( GL_UNIFORM_BUFFER, HGE_PIPELINE_MATRIX_BINDING, ubo );
    glBufferData( GL_UNIFORM_BUFFER, sizeof( transforms ), transforms, GL_DYNAMIC_DRAW );
    
    matrixIndexId = glGetUniformBlockIndex( currShader, "matrixBlock" );
    printGlError( "Accessing the Matrix Uniform Block" );
    
#ifdef DEBUG
    if ( matrixIndexId == GL_INVALID_INDEX ) {
        std::cerr
            << "ERROR: Incompatible shader detected. "
            << "Please ensure that the current shader has a uniform block "
            << "titled \"matrixBlock\" which contains the mat4 identifiers "
            << "\"modelMatrix\", \"viewMatrix\", "
            << "\"projMatrix\", and \"mvpMatrix\"."
            << std::endl;
        
        return;
    }
#endif
    
    glUniformBlockBinding( currShader, matrixIndexId, HGE_PIPELINE_MATRIX_BINDING );
    printGlError( "Sending Matrix Uniform Binding" );
    glBindBuffer( GL_UNIFORM_BUFFER, 0 );
}

/******************************************************************************
 * Pushing to the matrix stack
******************************************************************************/
void stockShaders::applyMatrix( e_matrixState s, const mat4& m ) {
    // Update the current MVP matrix
    transforms[ s ] = m;
    transforms[ 2 ] = transforms[ HGE_MODEL_MAT ] * transforms[ HGE_VP_MAT ];
    
    updateMatricesImpl();
}

void stockShaders::applyMatrix( const drawTransform& obj, const camera& cam ) {
    // Update the current MVP matrix
    transforms[ HGE_MODEL_MAT ] = obj.getModelMatrix();
    transforms[ HGE_VP_MAT ] = cam.getVPMatrix();
    transforms[ 2 ] = transforms[ HGE_MODEL_MAT ] * transforms[ HGE_VP_MAT ];
    
    updateMatricesImpl();
}

/******************************************************************************
 * Popping off a matrix from the stack
******************************************************************************/
void stockShaders::removeMatrix( e_matrixState s ) {
    transforms[ s ] = mat4( 1.f );
}

/******************************************************************************
 * Billboards
 ******************************************************************************/
void stockShaders::applyBillboardShader() {
    stockShaders::applyShader( bbShader.getProgramId() );
}

void stockShaders::setBillboardCam( const camera& c ) {
    glUniform3fv( camPosId, 1, c.getPos().v );
}

/******************************************************************************
 * Point Lights & Shadows
 ******************************************************************************/
void stockShaders::setPointLightMvp( const mat4& mvp ) {
    glUniformMatrix4fv( lightMatrixId, 1, GL_FALSE, (GLfloat*)mvp.m );
}

void stockShaders::applyPointLightShader() {
    stockShaders::applyShader( pointLightShader.getProgramId() );
}

void stockShaders::setPointLight( const ambientLight& a, const pointLight& p ) {
    glUniform1f(    ambIntId,       a.intensity );
    glUniform1f(    pointIntId,     p.intensity );
    glUniform1f(    pointLinearId,  p.linear );
    glUniform1f(    pointConstId,   p.constant );
    glUniform1f(    pointExpId,     p.exponential );
    glUniform4fv(   ambColorId,     1, a.color.v );
    glUniform4fv(   pointColorId,   1, p.color.v );
    glUniform3fv(   pointPosId,     1, p.pos.v );
}

void stockShaders::applyShadowShader() {
    stockShaders::applyShader( shadowShader.getProgramId() );
}

void stockShaders::applyFontShader() {
    stockShaders::applyShader( fontShader.getProgramId() );
}

void stockShaders::setFontColor( const vec4& v ) {
    glUniform4fv( fontColId, 1, v.v );
}

/******************************************************************************
 * Skybox
 ******************************************************************************/
void stockShaders::applySkyShader() {
    stockShaders::applyShader( skyShader.getProgramId() );
}

/******************************************************************************
 * Normal/Tangent/Bitangent Visualization
 ******************************************************************************/
void stockShaders::applyNbtShader() {
    stockShaders::applyShader( nbtShader.getProgramId() );
}

void stockShaders::showNormals( bool b ) {
    glUniform1i( nbtShowNormId, (int)b );
}

void stockShaders::showTangents( bool b ) {
    glUniform1i( nbtShowTangId, (int)b );
}

void stockShaders::showBitangents( bool b ) {
    glUniform1i( nbtShowBtngId, (int)b );
}

} // end harbinger namespace