/* 
 * File:   shader.h
 * Author: hammy
 *
 * Created on August 12, 2012, 10:29 PM
 */

#ifndef __HGE_SHADER_H__
#define __HGE_SHADER_H__

#include <vector>
#include "types.h"
#include "pipeline.h"
#include "resource.h"

namespace hge {

///////////////////////////////////////////////////////////////////////////////
//		OpenGL Shader Class
///////////////////////////////////////////////////////////////////////////////
class HGE_API shader : public resource {
    private:
        std::vector< GLuint > shaderIds;
        GLuint programId = 0;

    public:
        shader      ()              {}
        ~shader     ()              { unload(); }

        bool        isLoaded        () const { return (programId) ? true : false; }
        bool        load            ( const char* shaderFilePath, int shaderType );
        bool        loadBuffer      ( const char* shaderBuffer, int length, int shaderType );
        bool        compile         ();
        void        unload          ();
        GLuint      getProgramId    () const { return programId; }
        GLint       getVariableId   ( const char* );
};

} // end hge namespace

#endif	/* __HGE_SHADER_H__ */
