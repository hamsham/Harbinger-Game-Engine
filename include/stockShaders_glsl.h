/* 
 * File:   stockShaders_glsl.h
 * Author: hammy
 *
 * Created on July 11, 2013, 5:41 PM
 */

#ifndef __HGE_STOCKSHADERS_GLSL_H__
#define	__HGE_STOCKSHADERS_GLSL_H__

/******************************************************************************
 * Stock Shader Private Variables
 ******************************************************************************/
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
        gl_Position = vec4(mvpMatrix * vec4(-posVerts, 1.0)).xyww;
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
 * Normal/Tangent/Bitangent Visualizer
 ******************************************************************************/
const char enbtVS[] = R"***(
    #version 330
    
    layout ( std140 ) uniform matrixBlock {
        mat4 modelMatrix;
        mat4 vpMatrix;
        mat4 mvpMatrix;
    };

    layout ( location = 0 ) in vec3 posVerts;
    layout ( location = 1 ) in vec2 texVerts;
    layout ( location = 2 ) in vec3 normVerts;
    layout ( location = 3 ) in vec3 tangVerts;
    layout ( location = 4 ) in vec3 btngVerts;
    
    out vec4 normPos;
    out vec4 tangPos;
    out vec4 btngPos;
    
    void main() {
        gl_Position = mvpMatrix * vec4( posVerts, 1.0 );
        normPos     = mvpMatrix * vec4( normVerts, 0.0 );
        tangPos     = mvpMatrix * vec4( tangVerts, 0.0 );
        btngPos     = mvpMatrix * vec4( btngVerts, 0.0 );
    }
    
)***";

const char enbtGS[] = R"***(
    #version 330
    
    layout( triangles ) in;
    layout( line_strip, max_vertices = 21 ) out;
    
    uniform bool showEdges      = true;
    uniform bool showNormals    = true;
    uniform bool showTangents   = true;
    uniform bool showBitangents = true;
    
    in vec4 normPos[];
    in vec4 tangPos[];
    in vec4 btngPos[];
    out vec4 lineCol;
    
    void main() {
        
        for ( int i = 0; i < 3; ++i ) {
            vec4 origin = gl_in[i].gl_Position;
            
            if ( showBitangents ) {
                lineCol = vec4( 0.0, 0.0, 1.0, 1.0 );
                gl_Position = origin;
                EmitVertex();
                gl_Position = origin + btngPos[i];
                EmitVertex();
                EndPrimitive();
            }
            
            if ( showTangents ) {
                lineCol = vec4( 1.0, 0.0, 0.0, 1.0 );
                gl_Position = origin;
                EmitVertex();
                gl_Position = origin + tangPos[i];
                EmitVertex();
                EndPrimitive();
            }

            if ( showNormals ) {
                lineCol = vec4( 0.0, 1.0, 0.0, 1.0 );
                gl_Position = origin;
                EmitVertex();
                gl_Position = origin + normPos[i];
                EmitVertex();
                EndPrimitive();
            }
        }

        if ( showEdges ) {
            lineCol = vec4( 1.0, 0.0, 1.0, 1.0 );
            vec4 a = gl_in[0].gl_Position;
            vec4 b = gl_in[1].gl_Position;
            vec4 c = gl_in[2].gl_Position;

            gl_Position = a;
            EmitVertex();
            gl_Position = b;
            EmitVertex();
            EndPrimitive();

            gl_Position = b;
            EmitVertex();
            gl_Position = c;
            EmitVertex();
            EndPrimitive();

            gl_Position = c;
            EmitVertex();
            gl_Position = a;
            EmitVertex();
            EndPrimitive();
        }
    }
    
)***";

const char enbtFS[] = R"***(
    #version 330
    
    in vec4 lineCol;
    out vec4 fragCol;
    
    void main() {
        fragCol = lineCol;
    }
    
)***";

} // end anonymous namespace

#endif	/* __HGE_STOCKSHADERS_GLSL_H__ */

