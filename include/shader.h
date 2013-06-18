/* 
 * File:   shader.h
 * Author: hammy
 *
 * Created on August 12, 2012, 10:29 PM
 */

#ifndef __HGE_SHADER_H__
#define __HGE_SHADER_H__

#include <vector>
#include "pipeline.h"
#include "resource.h"

namespace hge {

///////////////////////////////////////////////////////////////////////////////
//		OpenGL Shader Class
///////////////////////////////////////////////////////////////////////////////
class HGE_API shader : virtual public resource {
    private:
        std::vector< GLuint > shaderIds;
        GLuint programId = 0;

    public:
        enum : GLint { INVALID_UNIFORM = -1 };
        
        shader      ()              {}
        ~shader     ()              { unload(); }

        bool        isLoaded        () const { return (programId) ? true : false; }
        bool        load            ( const char* shaderFilePath, int shaderType );
        bool        loadBuffer      ( const char* shaderBuffer, int length, int shaderType );
        bool        compile         ();
        void        unload          ();
        GLuint      getProgramId    () const { return programId; }
        GLint       getVariableId   ( const char* v ) { return glGetUniformLocation( programId, v ); }
};

} // end hge namespace

#endif	/* __HGE_SHADER_H__ */
