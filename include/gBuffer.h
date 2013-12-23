/* 
 * File:   gBuffer.h
 * Author: hammy
 *
 * Created on July 28, 2013, 11:00 PM
 */

#ifndef __HGE_GBUFFER_H__
#define	__HGE_GBUFFER_H__

#include "pipeline.h"

namespace hge {

class HGE_API gBuffer {
    public:
        enum buffer_target : int {
            GB_POSITION_BUFFER  = GL_COLOR_ATTACHMENT0,
            GB_DIFFUSE_BUFFER   = GL_COLOR_ATTACHMENT1,
            GB_NORMAL_BUFFER    = GL_COLOR_ATTACHMENT2,
            GB_RENDER_BUFFER    = 0
        };
        
    private:
        buffer_target   readBuffer          = GB_RENDER_BUFFER;
        GLuint          fbo                 = 0;
        GLuint          renderBuffer        = 0; // depth/stencil buffer
        GLuint          textures[ 3 ]       = { 0,0,0 }; // positions, colors, normals, composite
        vec2i           bufferRes;
        
    public:
        gBuffer         () {}
        gBuffer         ( const gBuffer& ) = delete;
        gBuffer         ( gBuffer&& );
        
        ~gBuffer        () { terminate(); }
        
        gBuffer&        operator = ( const gBuffer& ) = delete;
        gBuffer&        operator = ( gBuffer&& );
        
        bool init( const vec2i& windowRes );
        void terminate();
        
        const vec2i getBufferResolution() const { return bufferRes; }
        void setBufferResolution( const vec2i& res ) { init( res ); }
        
        
        /*
         * NOTE:
         * All miscellaneous draw calls (fonts, skybox, etc...) should be drawn
         * before using "bindForReading()" Doing so will allow all objects to
         * be rendered normally, as if the default framebuffer was already active
         */
        
        // Draw all geometry here, call GlClear()
        void bindForGeometryPass() const;
        
        // Light pass, draw fonts and skybox afterwards
        void bindForLightPass() const;
        
        void unbind() const;
        
        void setReadBuffer( buffer_target b ) { readBuffer = b; }
        
#ifdef DEBUG
        void drawBufferDebug() const;
#endif
};

} // end hge namespace

#endif	/* __HGE_GBUFFER_H__ */

