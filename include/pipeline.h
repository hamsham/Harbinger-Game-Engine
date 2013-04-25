/* 
 * File:   pipeline.h
 * Author: hammy
 *
 * Created on December 21, 2012, 1:49 PM
 */

#ifndef __HGE_GL_PIPELINE_H__
#define	__HGE_GL_PIPELINE_H__

#include <GL/glew.h>
#include <GL/glfw.h>
#include "types.h"

#ifndef HGE_PIPELINE_MATRIX_BINDING
    #define HGE_PIPELINE_MATRIX_BINDING 1
#endif /* HGE_PIPELINE_MATRIX_BINDING */

/******************************************************************************
 * OpenGL graphics pipeline functions
******************************************************************************/
namespace hge {
namespace pipeline {

enum e_attributes : GLint {
    INVALID_UNIFORM = -1,
    VERTEX_ATTRIB   = 0,
    TEXTURE_ATTRIB  = 1,
    NORMAL_ATTRIB   = 2,
    TANGENT_ATTRIB  = 3
};

// These are based off the enumerations defined in Assimp's "material.h" header
// Due to OpenGL limitations, no more than 32 enum types will be defined here
enum e_texType : GLint {
    HGE_TEXTURE_DEFAULT         = GL_TEXTURE0,
    HGE_TEXTURE_DIFFUSE         = GL_TEXTURE0,
    HGE_TEXTURE_SPECULAR        = GL_TEXTURE1,
    HGE_TEXTURE_AMBIENT         = GL_TEXTURE2,
    HGE_TEXTURE_EMMISSIVE       = GL_TEXTURE3,
    HGE_TEXTURE_HEIGHT          = GL_TEXTURE4,
    HGE_TEXTURE_NORMAL          = GL_TEXTURE5,
    HGE_TEXTURE_SHININESS       = GL_TEXTURE6,
    HGE_TEXTURE_OPACITY         = GL_TEXTURE7,
    HGE_TEXTURE_DISPLACEMENT    = GL_TEXTURE8,
    HGE_TEXTURE_LIGHTMAP        = GL_TEXTURE9,
    HGE_TEXTURE_REFLECTION      = GL_TEXTURE10,
    HGE_TEXTURE_SHADOWMAP       = GL_TEXTURE11,
    HGE_TEXTURE_AUX1            = GL_TEXTURE12,
    HGE_TEXTURE_AUX2            = GL_TEXTURE13,
    HGE_TEXTURE_AUX3            = GL_TEXTURE14,
    HGE_TEXTURE_AUX4            = GL_TEXTURE15,
    HGE_TEXTURE_AUX5            = GL_TEXTURE16,
    HGE_TEXTURE_AUX6            = GL_TEXTURE17,
    HGE_TEXTURE_AUX7            = GL_TEXTURE18,
    HGE_TEXTURE_AUX8            = GL_TEXTURE19,
    HGE_TEXTURE_AUX9            = GL_TEXTURE20,
    HGE_TEXTURE_AUX10           = GL_TEXTURE21,
    HGE_TEXTURE_AUX11           = GL_TEXTURE22,
    HGE_TEXTURE_AUX12           = GL_TEXTURE23,
    HGE_TEXTURE_AUX13           = GL_TEXTURE24,
    HGE_TEXTURE_AUX14           = GL_TEXTURE25,
    HGE_TEXTURE_AUX15           = GL_TEXTURE26,
    HGE_TEXTURE_AUX16           = GL_TEXTURE27,
    HGE_TEXTURE_AUX17           = GL_TEXTURE28,
    HGE_TEXTURE_AUX18           = GL_TEXTURE29,
    HGE_TEXTURE_AUX19           = GL_TEXTURE30,
    HGE_TEXTURE_AUX20           = GL_TEXTURE31,
    HGE_TEXTURE_UNKNOWN         = 0
};

enum e_matrixState : int {
    HGE_MODEL_MAT,
    HGE_VIEW_MAT,
    HGE_PROJ_MAT
};

// The matrix stack will only hold (at most) a built-in default matrix
// or one single user-applied matrix. Pass a NULL mat4 pointer to use the
// default system matrices (no scaling, rotation, or translation).
HGE_API void applyMatrix( e_matrixState s, const hamLibs::math::mat4& m = NULL );
HGE_API void removeMatrix( e_matrixState s );
HGE_API void applyShader( GLuint programId );
HGE_API void printGlErrorMsg( cstr msg, uint lineNum, cstr sourceFile );
HGE_API bool init();
HGE_API void terminate();

HGE_INLINE vec3 getWorldAxisX() { return vec3( 1.f, 0.f, 0.f ); }
HGE_INLINE vec3 getWorldAxisY() { return vec3( 0.f, 1.f, 0.f ); }
HGE_INLINE vec3 getWorldAxisZ() { return vec3( 0.f, 0.f, 1.f ); }

} // end pipeline namespace
} // end hge namespace

/******************************************************************************
 * OpenGL convenience function for error printing
******************************************************************************/
#ifdef DEBUG
	#define printGlError( x ) hge::pipeline::printGlErrorMsg( x, __LINE__, __FILE__ )
#else
	#define printGlError( x )
#endif

#endif	/* __HGE_GL_PIPELINE_H__ */
