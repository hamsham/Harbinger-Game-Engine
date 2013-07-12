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
#include "transformations.h"

namespace pipeline = hge::pipeline;
using hamLibs::math::mat4;

namespace {
/******************************************************************************
 * Plain, reusable vertex Shader
 ******************************************************************************/
const char plainVS[] = R"***(
    #version 330
    
    layout ( std140 ) uniform matrixBlock {
        mat4 modelMatrix;
        mat4 vpMatrix;
        mat4 mvpMatrix;
    };

    layout ( location = 0 ) in vec3 posVerts;
    layout ( location = 1 ) in vec2 texVerts;

    out vec2 texCoords;

    void main() {
        texCoords = texVerts;
        gl_Position = mvpMatrix * vec4(posVerts, 1.0);
    }
)***";

/******************************************************************************
 * Shadow Shader
 ******************************************************************************/
const char shadowFS[] = R"***(
    #version 330

    uniform sampler2D shadowMap;

    in vec2 texCoords;
    out vec4 fragCol;

    void main() {
        fragCol = texture( shadowMap, texCoords );
    }
)***";

/******************************************************************************
 * Point Light Shader
 ******************************************************************************/
const char pointVS[] = R"***(
    #version 330
    
    layout ( std140 ) uniform matrixBlock {
        mat4 modelMatrix;
        mat4 vpMatrix;
        mat4 mvpMatrix;
    };

    layout ( location = 0 ) in vec3 posVerts;
    layout ( location = 1 ) in vec2 texVerts;
    layout ( location = 2 ) in vec3 nrmVerts;
    layout ( location = 3 ) in vec3 tngVerts;
    layout ( location = 4 ) in vec3 btngVerts;

    uniform mat4 lightMVP;

    out vec2 texCoords;
    out vec3 nrmCoords;
    out vec3 tngCoords;
    out vec3 btngCoords;
    out vec3 worldPos;
    out vec4 lightPos;

    void main() {
        texCoords   = texVerts;
        worldPos    = vec3(modelMatrix * vec4(posVerts, 1.0)).xyz;
        lightPos    = lightMVP * vec4( posVerts, 1.0 );
        gl_Position = mvpMatrix * vec4(posVerts, 1.0);

        nrmCoords   = normalize(modelMatrix * vec4(nrmVerts, 0.0)).xyz;
        tngCoords   = normalize(modelMatrix * vec4(tngVerts, 0.0)).xyz;
        btngCoords  = normalize(modelMatrix * vec4(btngVerts, 0.0)).xyz;
    }
)***";

const char pointFS[] = R"***(
    #version 330

    struct s_ambientLight {
        vec4    color;
        float   intensity;
    };

    struct s_pointLight {
        float   constant;
        float   exponential;
        float   intensity;
        float   linear;
        vec3    position;
        vec4    color;
    };

    uniform sampler2D       texSampler;
    uniform sampler2D       shadowMap;
    uniform sampler2D       normalMap;
    uniform s_ambientLight  ambientLight;
    uniform s_pointLight    pointLight;

    in vec2 texCoords;
    in vec3 nrmCoords;
    in vec3 tngCoords;
    in vec3 btngCoords;
    in vec3 worldPos;
    in vec4 lightPos;

    out vec4 fragCol;

    vec4 calcAmbientLight( in s_ambientLight amb ) {
        return amb.color * amb.intensity;
    }

    float calcShadowAmount( in vec4 lightPosition ) {
        vec3 projectionCoords   = (lightPosition.xyz / lightPosition.w) * 0.5;
        vec3 uvCoords           = projectionCoords + 0.5;

        return texture( shadowMap, uvCoords.xy ).x < uvCoords.z+0.00001
            ? 0.0 : 1.0;
    }

    vec4 calcPointLight( in s_pointLight p, in vec3 normals ) {
        vec3 lightDir   = worldPos - p.position;
        float distance  = length( lightDir );
        lightDir        = -normalize( lightDir );

        float diffuse = max( 0.0, dot( normals, lightDir ) );
        float attenuation
            = p.constant
            + ( p.linear * distance )
            + ( p.exponential * distance*distance );

        return p.color * p.intensity * diffuse / attenuation;
    }

    vec3 calcBumpedNormal() {
//        vec3 tangent    = normalize( tngCoords - dot(tngCoords, nrmCoords) * nrmCoords );
//        vec3 bitangent = cross( tangent, nrmCoords );
//        vec3 bumpNormal = 2.0 * texture( normalMap, texCoords ).rgb - 1.0;
//        mat3 tbn        = mat3( tangent, bitangent, nrmCoords );
        
        vec3 bumpNormal = 2.0 * texture( normalMap, texCoords ).rgb - 1.0;
        mat3 tbn        = mat3( tngCoords, btngCoords, nrmCoords );

        return normalize( tbn * bumpNormal );
    }

    void main() {
        fragCol
            = texture2D( texSampler, texCoords ) * (
                calcAmbientLight( ambientLight ) + (
                    calcPointLight( pointLight, calcBumpedNormal() ) *
                    calcShadowAmount( lightPos )
                )
            );
    }
)***";

/******************************************************************************
 * Skybox shader
 ******************************************************************************/
const char skyVS[] = R"***(
    #version 330
    
    layout ( std140 ) uniform matrixBlock {
        mat4 modelMatrix;
        mat4 vpMatrix;
        mat4 mvpMatrix;
    };

    layout ( location = 0 ) in vec3 posVerts;

    out vec3 texCoords;

    void main() {
        texCoords = posVerts;
        gl_Position = mvpMatrix * vec4(-posVerts, 1.0);
    }
)***";

const char skyFS[] = R"***(
    #version 330

    in vec3 texCoords;
    out vec4 fragCol;

    uniform samplerCube cubeTex;

    void main() {
        fragCol = texture( cubeTex, texCoords );
    }
)***";

/******************************************************************************
 * Font shader
 ******************************************************************************/
const char fontFS[] = R"***(
    #version 330

    in vec2 texCoords;
    out vec4 fragCol;

    uniform sampler2D texSampler;
    uniform vec4 color;

    void main() {
        vec4 fontTexture = vec4( texture2D( texSampler, texCoords ).r );
        fragCol = fontTexture * color;
    }
)***";

/******************************************************************************
 * Font shader
 ******************************************************************************/
const char billboardVS[] = R"***(
    #version 330
    
    layout ( std140 ) uniform matrixBlock {
        mat4 modelMatrix;
        mat4 vpMatrix;
        mat4 mvpMatrix;
    };

    layout ( location = 0 ) in vec3 posVerts;

    void main() {
        gl_Position = vec4( posVerts, 1.0 );
    }
)***";

const char billboardGS[] = R"***(
    #version 330
    
    layout ( std140 ) uniform matrixBlock {
        mat4 modelMatrix;
        mat4 vpMatrix;
        mat4 mvpMatrix;
    };
    
    uniform vec3 camPos;

    layout ( points ) in;
    layout ( triangle_strip ) out;
    layout ( max_vertices = 4 ) out;

    out vec2 texCoord;

    void main() {
        vec3 inPos      = gl_in[0].gl_Position.xyz;
        vec3 toCamera   = normalize( camPos - inPos );
        vec3 upVec      = vec3( 0.0, 1.0, 0.0 );
        vec3 rightVec   = cross( toCamera, upVec );

        inPos       -= ( rightVec * 0.5 );
        gl_Position = vpMatrix * vec4( inPos, 1.0 );
        texCoord    = vec2( 0.0, 0.0 );
        EmitVertex();

        inPos.y     += 1.0;
        gl_Position = vpMatrix * vec4( inPos, 1.0 );
        texCoord    = vec2( 0.0, 1.0 );
        EmitVertex();

        inPos.y     -= 1.0;
        inPos       += rightVec;
        gl_Position = vpMatrix * vec4( inPos, 1.0 );
        texCoord    = vec2( 1.0, 0.0 );
        EmitVertex();

        inPos.y     += 1.0;
        gl_Position = vpMatrix * vec4( inPos, 1.0 );
        texCoord    = vec2( 1.0, 1.0 );
        EmitVertex();

        EndPrimitive();
    }
)***";

const char billboardFS[] = R"***(
    #version 330

    uniform sampler2D texSampler;

    in vec2 texCoord;
    out vec4 fragCol;

    void main() {
        fragCol = texture2D( texSampler, texCoord );

        if ( fragCol.r == 0 && fragCol.g == 0 && fragCol.b == 0 )
            discard;
    }
)***";

/******************************************************************************
 * Shader Manager Private Variables
 ******************************************************************************/
//Point Light Shader
hge::shader     pointLightShader;
GLint           ambColorId      = 0;
GLint           ambIntId        = 0;
GLint           pointColorId    = 0;
GLint           pointIntId      = 0;
GLint           pointLinearId   = 0;
GLint           pointConstId    = 0;
GLint           pointExpId      = 0;
GLint           pointPosId      = 0;
GLint           shadowId        = 0;
GLint           lightMatrixId   = 0;
GLint           textureId       = 0;
GLint           normalId        = 0;
// Shadow Shader
hge::shader     shadowShader;
GLint           shadowMapId     = 0;
// Skybox Shader
hge::shader     skyShader;
GLint           skyTexId        = 0;
// Font Shader Setup
hge::shader     fontShader;
GLint           fontColId       = 0;
GLint           fontSampler     = 0;
// Billboard Setup
hge::shader     bbShader;
GLint           camPosId        = 0;
GLint           bbTexSampler    = 0;

// Commonly shared Uniform buffer object
GLuint ubo = 0;
GLuint matrixIndexId = GL_INVALID_INDEX;
GLuint currShader = 0;

// Creating an array of four 4x4 Matrices. Model, View, Projection, and MVP
mat4 transforms[ 3 ] = { mat4( 1.f ) };


inline void updateMatricesImpl() {
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

bool initPointLightShader();
bool initShadowShader();
bool initSkyShader();
bool initFontShader();
bool initBillboardShader();

/******************************************************************************
 * POINT LIGHT SHADER
 ******************************************************************************/
bool initPointLightShader() {
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
bool initShadowShader() {
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
bool initSkyShader() {
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
bool initFontShader() {
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
bool initBillboardShader() {
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
 * Shader Manager Initialization
 ******************************************************************************/
} // end anonymous namespace
namespace hge {
bool stockShaders::init() {
    if ( ubo != 0 ) {
        // Return if the pipeline is already initialized
        return true;
    }

    glGenBuffers( 1, &ubo );
    printGlError( "Creating pipeline uniform block" );

    if ( ubo == 0 ) {
        std::cerr
            << "An error occurred while initializing the graphics pipeline."
            << std::endl;
        return false;
    }
    
    if (
            !shadowShader.loadBuffer( plainVS, sizeof( plainVS ), GL_VERTEX_SHADER )
    ||      !shadowShader.loadBuffer( shadowFS, sizeof( shadowFS ), GL_FRAGMENT_SHADER )
    ||      !shadowShader.compile()
            
    ||      !pointLightShader.loadBuffer( pointVS, sizeof( pointVS ), GL_VERTEX_SHADER )
    ||      !pointLightShader.loadBuffer( pointFS, sizeof( pointFS ), GL_FRAGMENT_SHADER )
    ||      !pointLightShader.compile()
            
    ||      !skyShader.loadBuffer( skyVS, sizeof( skyVS ), GL_VERTEX_SHADER )
    ||      !skyShader.loadBuffer( skyFS, sizeof( skyFS ), GL_FRAGMENT_SHADER )
    ||      !skyShader.compile()
            
    ||      !fontShader.loadBuffer( plainVS, sizeof( plainVS ), GL_VERTEX_SHADER )
    ||      !fontShader.loadBuffer( fontFS, sizeof( fontFS ), GL_FRAGMENT_SHADER )
    ||      !fontShader.compile()
            
    ||      !bbShader.loadBuffer( billboardVS, sizeof( billboardVS ), GL_VERTEX_SHADER )
    ||      !bbShader.loadBuffer( billboardGS, sizeof( billboardGS ), GL_GEOMETRY_SHADER )
    ||      !bbShader.loadBuffer( billboardFS, sizeof( billboardFS ), GL_FRAGMENT_SHADER )
    ||      !bbShader.compile()
    ) {
        std::cout << "An error occurred while compiling a shader" << std::endl;
        terminate();
        return false;
    }
    printGlError("Shader compilation error");
    
    if (    !initPointLightShader()
    ||      !initShadowShader()
    ||      !initSkyShader()
    ||      !initFontShader()
    ||      !initBillboardShader()
    ) {
        terminate();
        return false;
    }
    
    return true;
}

/******************************************************************************
 * Shader Manager Termination
 ******************************************************************************/
void stockShaders::terminate() {
    if ( ubo == 0 )
        return;
    
    glDeleteBuffers( 1, &ubo );
    ubo = currShader = 0;
    matrixIndexId = GL_INVALID_INDEX;
    
    pointLightShader.unload();
    shadowShader.unload();
    fontShader.unload();
    skyShader.unload();
    bbShader.unload();
    
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

void stockShaders::applySkyShader() {
    stockShaders::applyShader( skyShader.getProgramId() );
}

} // end harbinger namespace