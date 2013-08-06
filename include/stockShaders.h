/* 
 * File:   shaderMgr.h
 * Author: hammy
 *
 * Created on May 14, 2013, 12:25 AM
 */

#ifndef __HGE_STOCK_SHADERS_H__
#define	__HGE_STOCK_SHADERS_H__

#include <string>
#include "types.h"
#include "pipeline.h"
#include "shader.h"
#include "camera.h"

namespace hge {

/******************************************************************************
 * Stock Shader Base Class
******************************************************************************/
class HGE_API stockShader {
    protected:
        pipeline* currPipeline = nullptr;
        shader program;
        
    public:
        stockShader             () {}
        stockShader             ( const stockShader& ) = delete;
        stockShader             ( stockShader&& );
        virtual ~stockShader    () {}
        
        stockShader& operator = ( const stockShader& ) = delete;
        stockShader& operator = ( stockShader&& );
        
        virtual bool init       ( pipeline* const p ) = 0;
        virtual void terminate  () = 0;
        
        void        activate    ();
};

inline void stockShader::activate() {
    currPipeline->applyStockShader( program.getProgramId() );
}

/******************************************************************************
 * Point Light Shader
******************************************************************************/
class HGE_API pointLightShader final : public stockShader {
    private:
        GLint           ambColorId      = 0;
        GLint           ambIntId        = 0;
        GLint           pointColorId    = 0;
        GLint           pointIntId      = 0;
        GLint           pointLinearId   = 0;
        GLint           pointConstId    = 0;
        GLint           pointExpId      = 0;
        GLint           pointPosId      = 0;
        GLint           lightMatrixId   = 0;
        
    public:
        pointLightShader                () {}
        pointLightShader                ( const pointLightShader& ) = delete;
        pointLightShader                ( pointLightShader&& );
        ~pointLightShader               () { terminate(); }
        
        pointLightShader& operator =    ( const pointLightShader& ) = delete;
        pointLightShader& operator =    ( pointLightShader&& );
        
        bool init                       ( pipeline* const p );
        void terminate                  ();
        
        void setPointLight              ( const pointLight& );
        void setAmbientLight            ( const ambientLight& );
        void setLightMvp                ( const mat4& );
};

/******************************************************************************
 * Shadow Map Shader
******************************************************************************/
class HGE_API shadowShader final : public stockShader {
    public:
        shadowShader                () {}
        shadowShader                ( const shadowShader& ) = delete;
        shadowShader                ( shadowShader&& ) = default;
        ~shadowShader               () { terminate(); }
        
        shadowShader& operator =    ( const shadowShader& ) = delete;
        shadowShader& operator =    ( shadowShader&& ) = default;
        
        bool init                   ( pipeline* const p );
        void terminate              () { program.unload(); }
};

/******************************************************************************
 * Skybox Shader
******************************************************************************/
class HGE_API skyShader final : public stockShader {
    public:
        skyShader               () {}
        skyShader               ( const skyShader& ) = delete;
        skyShader               ( skyShader&& ) = default;
        ~skyShader              () { terminate(); }
        
        skyShader& operator =   ( const skyShader& ) = delete;
        skyShader& operator =   ( skyShader&& ) = default;
        
        bool init               ( pipeline* const p );
        void terminate          () { program.unload(); }
};

/******************************************************************************
 * Font Shader
 * 
 * NOTE: These must be drawn after a skybox due to transparency issues
******************************************************************************/
class HGE_API fontShader final : public stockShader {
    private:
        GLint fontColId = 0;
        
    public:
        fontShader              () {}
        fontShader              ( const fontShader& ) = delete;
        fontShader              ( fontShader&& );
        ~fontShader             () { terminate(); }
        
        fontShader& operator =  ( const fontShader& ) = delete;
        fontShader& operator =  ( fontShader&& );
        
        bool init               ( pipeline* const p );
        void terminate          ();
        
        void setFontColor       ( const vec4& v ) { glUniform4fv( fontColId, 1, v.v ); }
};

/******************************************************************************
 * Billboard Shader
******************************************************************************/
class HGE_API billboardShader final : public stockShader {
    private:
        GLint camPosId = 0;
        
    public:
        billboardShader             () {}
        billboardShader             ( const billboardShader& ) = delete;
        billboardShader             ( billboardShader&& );
        ~billboardShader            () { terminate(); }
        
        billboardShader& operator = ( const billboardShader& ) = delete;
        billboardShader& operator = ( billboardShader&& );
        
        bool init                   ( pipeline* const p );
        void terminate              ();
        
        void setCameraPos           ( const camera& c ) { glUniform3fv( camPosId, 1, c.getPos().v ); }
};

/******************************************************************************
 * Debug Shader to view Vertices, normals, tangents, and bitangents
 * 
 * NOTE:
 *      The NBT shader only works with objects that have been drawn using
 *      the GL_TRIANGLE* specifiers. Objects drawn with GL_LINES or
 *      GL_POINTS specifiers will not work
******************************************************************************/
class HGE_API vnbtShader final : public stockShader {
    private:
        GLint showVertId = 0;
        GLint showNormId = 0;
        GLint showTangId = 0;
        GLint showBtngId = 0;
        
    public:
        vnbtShader              () {}
        vnbtShader              ( const vnbtShader& ) = delete;
        vnbtShader              ( vnbtShader&& );
        
        vnbtShader& operator =  ( const vnbtShader& ) = delete;
        vnbtShader& operator =  ( vnbtShader&& );
        
        ~vnbtShader             () { terminate(); }
        
        bool init               ( pipeline* const p );
        void terminate          ();
        
        void    showVertices    ( bool b ) { glUniform1i( showVertId, (int)b ); }
        void    showNormals     ( bool b ) { glUniform1i( showNormId, (int)b ); }
        void    showTangents    ( bool b ) { glUniform1i( showTangId, (int)b ); }
        void    showBitangents  ( bool b ) { glUniform1i( showBtngId, (int)b ); }
};

} // end hge namespace

#endif	/* __HGE_STOCK_SHADERS_H__ */
