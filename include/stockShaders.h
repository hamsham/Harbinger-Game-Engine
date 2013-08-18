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

/*
 * Stock shader move operators unload their program using the "program" variable
 * to avoid redundant calculations through variables with static data.
 */

namespace hge {

/******************************************************************************
 * Stock Shader Base Class
******************************************************************************/
class HGE_API stockShader {
    protected:
        shader program;
        
    public:
        stockShader     () {}
        stockShader     ( const stockShader& ) = delete;
        stockShader     ( stockShader&& );
        
        virtual ~stockShader        () {}
        
        stockShader&    operator =  ( const stockShader& ) = delete;
        stockShader&    operator =  ( stockShader&& );
        
        virtual bool    init        () = 0;
        virtual void    terminate   () { program.unload(); }
        
        GLuint          getProgramId() const { return program.getProgramId(); }
};

/******************************************************************************
 * Basic Shader
******************************************************************************/
class HGE_API plainShader final : public stockShader {
    public:
        plainShader     () {}
        plainShader     ( const plainShader& ) = delete;
        plainShader     ( plainShader&& ) = default;
        ~plainShader    () { terminate(); }
        
        plainShader&    operator =  ( const plainShader& ) = delete;
        plainShader&    operator =  ( plainShader&& ) = default;
        
        bool            init        ();
        void            terminate   () { program.unload(); }
};

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
        
        bool init                       ();
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
        
        bool init                   ();
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
        
        bool init               ();
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
        
        bool init               ();
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
        
        bool init                   ();
        void terminate              ();
        
        void setTargetPos           ( const vec3& target ) { glUniform3fv( camPosId, 1, target.v ); }
};

/******************************************************************************
 * Debug Shader to view Vertices, normals, tangents, and bitangents
 * 
 * NOTE:
 *      The NBT shader only works with objects that have been drawn using
 *      the GL_TRIANGLE* specifiers. Objects drawn with GL_LINES or
 *      GL_POINTS specifiers will not work
******************************************************************************/
class HGE_API enbtShader final : public stockShader {
    private:
        GLint showEdgeId = 0;
        GLint showNormId = 0;
        GLint showTangId = 0;
        GLint showBtngId = 0;
        
    public:
        enbtShader              () {}
        enbtShader              ( const enbtShader& ) = delete;
        enbtShader              ( enbtShader&& );
        
        enbtShader& operator =  ( const enbtShader& ) = delete;
        enbtShader& operator =  ( enbtShader&& );
        
        ~enbtShader             () { terminate(); }
        
        bool init               ();
        void terminate          ();
        
        void    showEdges       ( bool b ) { glUniform1i( showEdgeId, (int)b ); }
        void    showNormals     ( bool b ) { glUniform1i( showNormId, (int)b ); }
        void    showTangents    ( bool b ) { glUniform1i( showTangId, (int)b ); }
        void    showBitangents  ( bool b ) { glUniform1i( showBtngId, (int)b ); }
};

/******************************************************************************
 * DEFERRED RENDERING-RELATED SHADERS
******************************************************************************/
/******************************************************************************
 * Deferred Renderer Geometry Pass
******************************************************************************/
class HGE_API dsGeometryShader final : public stockShader {
    public:
        dsGeometryShader    () {}
        dsGeometryShader    ( const dsGeometryShader& ) = delete;
        dsGeometryShader    ( dsGeometryShader&& ) = default;
        
        dsGeometryShader&   operator =  ( const dsGeometryShader& ) = delete;
        dsGeometryShader&   operator =  ( dsGeometryShader&& ) = default;
        
        ~dsGeometryShader   () { terminate(); }
        
        bool init();
};

/******************************************************************************
 * Deferred Renderer Light Pass for instanced lights
******************************************************************************/
class HGE_API dsLightShader final : public stockShader {
    private:
        GLint resolutionId  = 0;
        
    public:
        dsLightShader() {}
        dsLightShader   ( const dsLightShader& ) = delete;
        dsLightShader   ( dsLightShader&& );
        
        dsLightShader&  operator =  ( const dsLightShader& ) = delete;
        dsLightShader&  operator =  ( dsLightShader&& );
        
        ~dsLightShader  () { terminate(); }
        
        bool init       ( const vec2i& gBufferResolution );
        bool init       () { return init( vec2i( 0,0 ) ); }
        void terminate  ();
        
        void setInputResolution( const vec2i& );
};

/******************************************************************************
 * Deferred Renderer Stencil Pass
******************************************************************************/
class HGE_API dsNullShader final : public stockShader {
    public:
        dsNullShader    () {}
        dsNullShader    ( const dsNullShader& ) = delete;
        dsNullShader    ( dsNullShader&& ) = default;
        
        dsNullShader&   operator =  ( const dsNullShader& ) = delete;
        dsNullShader&   operator =  ( dsNullShader&& ) = default;
        
        ~dsNullShader   () { terminate(); }
        
        bool init();
};

} // end hge namespace

#endif	/* __HGE_STOCK_SHADERS_H__ */
