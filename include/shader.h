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
class HGE_API c_shader : virtual public c_resource {
    private:
        std::vector< GLuint > shaderID;
        GLuint progID;

    public:
        enum : GLint { INVALID_UNIFORM = -1 };
        
        c_shader        ();
        ~c_shader       ();

        bool            isLoaded        () const { return (progID) ? true : false; }
        bool            load            ( const char* shaderFilePath, int shaderType );
        bool            loadBuffer      ( const char* shaderBuffer, int length, int shaderType );
        bool            compile         ();
        void            unload          ();
        GLuint          getProgramId    () const { return progID; }
        GLint           getVariableId   ( const char* v ) { return glGetUniformLocation( progID, v ); }
};

} // end hge namespace

#endif	/* __HGE_SHADER_H__ */
