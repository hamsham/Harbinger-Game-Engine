/* 
 * File:   pipeline.h
 * Author: hammy
 *
 * Created on December 21, 2012, 1:49 PM
 */

#ifndef __HGE_GL_PIPELINE_H__
#define	__HGE_GL_PIPELINE_H__

#include <GL/glew.h>
#include "types.h"

#ifndef HGE_PIPELINE_MATRIX_BINDING
    #define HGE_PIPELINE_MATRIX_BINDING 1
#endif /* HGE_PIPELINE_MATRIX_BINDING */

namespace hge {

/******************************************************************************
 * OpenGL graphics pipeline functions
******************************************************************************/
class HGE_API pipeline {
    public:
        enum matrixType : int {
            HGE_MODEL_MAT   = 0,
            HGE_VP_MAT      = 1
        };
        
        enum drawMode : GLenum {
            HGE_DEFAULT                     = (GLenum)-1,
            HGE_POINTS                      = GL_POINTS,
            HGE_LINES                       = GL_LINES,
            HGE_LINE_STRIP                  = GL_LINE_STRIP,
            HGE_LINE_LOOP                   = GL_LINE_LOOP,
            HGE_LINE_STRIP_ADJACENCY        = GL_LINE_STRIP_ADJACENCY,
            HGE_TRIANGLES                   = GL_TRIANGLES,
            HGE_TRIANGLES_ADJACENCY         = GL_TRIANGLES_ADJACENCY,
            HGE_TRIANGLE_FAN                = GL_TRIANGLE_FAN,
            HGE_TRIANGLE_STRIP              = GL_TRIANGLE_STRIP,
            HGE_TRIANGLE_STRIP_ADJACENCY    = GL_TRIANGLE_STRIP_ADJACENCY
        };

        enum attribute : GLint {
            HGE_ATTRIB_INVALID      = -1,
            HGE_ATTRIB_VERTEX       = 0,
            HGE_ATTRIB_TEXTURE      = 1,
            HGE_ATTRIB_NORMAL       = 2,
            HGE_ATTRIB_TANGENT      = 3,
            HGE_ATTRIB_BITANGENT    = 4
        };

        // These are based off the enumerations defined in Assimp's "material.h" header
        // Due to OpenGL limitations, no more than 32 enum types will be defined here.
        // Samplers are provided for aiding in the setup of shader sampler locations.
        enum textureType : GLint {
            HGE_TEXTURE_DEFAULT         = GL_TEXTURE0,  HGE_SAMPLER_DEFAULT         = 0,
            HGE_TEXTURE_DIFFUSE         = GL_TEXTURE0,  HGE_SAMPLER_DIFFUSE         = 0,
            HGE_TEXTURE_SPECULAR        = GL_TEXTURE1,  HGE_SAMPLER_SPECULAR        = 1,
            HGE_TEXTURE_AMBIENT         = GL_TEXTURE2,  HGE_SAMPLER_AMBIENT         = 2,
            HGE_TEXTURE_EMMISSIVE       = GL_TEXTURE3,  HGE_SAMPLER_EMMISSIVE       = 3,
            HGE_TEXTURE_HEIGHT          = GL_TEXTURE4,  HGE_SAMPLER_HEIGHT          = 4,
            HGE_TEXTURE_NORMAL          = GL_TEXTURE5,  HGE_SAMPLER_NORMAL          = 5,
            HGE_TEXTURE_SHININESS       = GL_TEXTURE6,  HGE_SAMPLER_SHININESS       = 6,
            HGE_TEXTURE_OPACITY         = GL_TEXTURE7,  HGE_SAMPLER_OPACITY         = 7,
            HGE_TEXTURE_DISPLACEMENT    = GL_TEXTURE8,  HGE_SAMPLER_DISPLACEMENT    = 8,
            HGE_TEXTURE_LIGHTMAP        = GL_TEXTURE9,  HGE_SAMPLER_LIGHTMAP        = 9,
            HGE_TEXTURE_REFLECTION      = GL_TEXTURE10, HGE_SAMPLER_REFLECTION      = 10,
            HGE_TEXTURE_SHADOWMAP       = GL_TEXTURE11, HGE_SAMPLER_SHADOWMAP       = 11,
            HGE_TEXTURE_AUX1            = GL_TEXTURE12, HGE_SAMPLER_AUX1            = 12,
            HGE_TEXTURE_AUX2            = GL_TEXTURE13, HGE_SAMPLER_AUX2            = 13,
            HGE_TEXTURE_AUX3            = GL_TEXTURE14, HGE_SAMPLER_AUX3            = 14,
            HGE_TEXTURE_AUX4            = GL_TEXTURE15, HGE_SAMPLER_AUX4            = 15,
            HGE_TEXTURE_AUX5            = GL_TEXTURE16, HGE_SAMPLER_AUX5            = 16,
            HGE_TEXTURE_AUX6            = GL_TEXTURE17, HGE_SAMPLER_AUX6            = 17,
            HGE_TEXTURE_AUX7            = GL_TEXTURE18, HGE_SAMPLER_AUX7            = 18,
            HGE_TEXTURE_AUX8            = GL_TEXTURE19, HGE_SAMPLER_AUX8            = 19,
            HGE_TEXTURE_AUX9            = GL_TEXTURE20, HGE_SAMPLER_AUX9            = 20,
            HGE_TEXTURE_AUX10           = GL_TEXTURE21, HGE_SAMPLER_AUX10           = 21,
            HGE_TEXTURE_AUX11           = GL_TEXTURE22, HGE_SAMPLER_AUX11           = 22,
            HGE_TEXTURE_AUX12           = GL_TEXTURE23, HGE_SAMPLER_AUX12           = 23,
            HGE_TEXTURE_AUX13           = GL_TEXTURE24, HGE_SAMPLER_AUX13           = 24,
            HGE_TEXTURE_AUX14           = GL_TEXTURE25, HGE_SAMPLER_AUX14           = 25,
            HGE_TEXTURE_AUX15           = GL_TEXTURE26, HGE_SAMPLER_AUX15           = 26,
            HGE_TEXTURE_AUX16           = GL_TEXTURE27, HGE_SAMPLER_AUX16           = 27,
            HGE_TEXTURE_AUX17           = GL_TEXTURE28, HGE_SAMPLER_AUX17           = 28,
            HGE_TEXTURE_AUX18           = GL_TEXTURE29, HGE_SAMPLER_AUX18           = 29,
            HGE_TEXTURE_AUX19           = GL_TEXTURE30, HGE_SAMPLER_AUX19           = 30,
            HGE_TEXTURE_AUX20           = GL_TEXTURE31, HGE_SAMPLER_AUX20           = 31,
            HGE_TEXTURE_UNKNOWN         = 0
        };
        
    private:
        // Commonly shared Uniform buffer object
        GLuint ubo = 0;
        GLuint currShader = 0;
        GLuint matrixIndexId = GL_INVALID_INDEX;
        // Array to contain the current model, view-projection, and mvp matrix transformations
        mat4* transforms = nullptr;
        
        // Shader Matrix Updating
        void updateMatricesImpl();
        
    public:
        pipeline() {}
        
        pipeline( const pipeline& ) = delete;
        pipeline( pipeline&& ) = delete;
        
        virtual ~pipeline() { terminate(); }
        
        pipeline& operator = ( const pipeline& ) = delete;
        pipeline& operator = ( pipeline&& ) = delete;
        
        bool init();
        void terminate();
        
        void applyStockShader( GLint shaderId );
        
        /******************************************************************************
         * Matrix UBO Modification
        ******************************************************************************/
        // The matrix stack will only hold (at most) a single user-applied matrix of
        // each type (model, view, and projection).
        void    applyMatrix         ( matrixType s, const mat4& m );
        void    applyMatrix         ( const drawTransform&, const camera& );
        void    removeMatrix        ( matrixType s );

        /******************************************************************************
         * OpenGL Display Pipeline Functions
        ******************************************************************************/
        static      void enablePlainVertexAttribs();
        static      void enableBumpVertexAttribs();

        static      void printErrorMsg( const char* msg, unsigned lineNum, const char* sourceFile );

        static constexpr vec3 getWorldAxisX() { return vec3( 1.f, 0.f, 0.f ); }
        static constexpr vec3 getWorldAxisY() { return vec3( 0.f, 1.f, 0.f ); }
        static constexpr vec3 getWorldAxisZ() { return vec3( 0.f, 0.f, 1.f ); }
        
    }; // end pipeline
} // end hge namespace

/******************************************************************************
 * OpenGL convenience function for error printing
******************************************************************************/
#ifdef DEBUG
	#define printGlError( x ) hge::pipeline::printErrorMsg( x, __LINE__, __FILE__ )
#else
	#define printGlError( x )
#endif

#endif	/* __HGE_GL_PIPELINE_H__ */
