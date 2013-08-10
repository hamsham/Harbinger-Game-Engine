/* 
 * File:   gBuffer.cpp
 * Author: hammy
 * 
 * Created on July 28, 2013, 11:00 PM
 */

#include <iostream>
#include "gBuffer.h"

namespace hge {

/******************************************************************************
 * G-BUFFER
******************************************************************************/
gBuffer::gBuffer( gBuffer&& gb ) :
    readBuffer( gb.readBuffer ),
    fbo( gb.fbo ),
    renderBuffer{ gb.renderBuffer[0], gb.renderBuffer[1] },
    textures{ gb.textures[0], gb.textures[1], gb.textures[2] },
    bufferRes( gb.bufferRes )
{
    gb.readBuffer = GB_RENDER_BUFFER;
    gb.fbo = 0;
    gb.renderBuffer[0] = gb.renderBuffer[1] = 0;
    gb.textures[0] = gb.textures[1] = gb.textures[2] = 0;
    gb.bufferRes = vec2i( 0,0 );
}

gBuffer& gBuffer::operator =( gBuffer&& gb ) {
    readBuffer = gb.readBuffer;
    gb.readBuffer = GB_RENDER_BUFFER;
    
    fbo = gb.fbo;
    gb.fbo = 0;
    
    renderBuffer[0] = gb.renderBuffer[0];
    renderBuffer[1] = gb.renderBuffer[1];
    gb.renderBuffer[0] = gb.renderBuffer[1] = 0;
    
    textures[0] = gb.textures[0];
    textures[1] = gb.textures[1];
    textures[2] = gb.textures[2];
    gb.textures[0] = gb.textures[1] = gb.textures[2] = 0;
    
    bufferRes = gb.bufferRes;
    gb.bufferRes = vec2i( 0,0 );
}

/******************************************************************************
 * G-BUFFER INITIALIZATION
******************************************************************************/
bool gBuffer::init( const vec2i& windowRes ) {
    // don't delete the FBO if changing resolution
    if ( !fbo ) {
        // Create the FBO
        glGenFramebuffers( 1, &fbo );
        glGenRenderbuffers( 2, renderBuffer );
        glGenTextures( 3, textures );

        glBindFramebuffer( GL_DRAW_FRAMEBUFFER, fbo );

        if ( !fbo || !renderBuffer[0] || !renderBuffer[1] ) {
            terminate();
            std::cerr << "ERROR: Unable to create storage for the G-Buffer." << std::endl;
            return false;
        }
    }
    
    // initialize all textures except for the depth texture
    for ( unsigned i = 0; i < 3; ++i ) {
        // bind the texture
        glBindTexture( GL_TEXTURE_2D, textures[ i ] );
        
        // allocate space for the texture data
        glTexImage2D(
            GL_TEXTURE_2D, 0, GL_RGB16F, windowRes[0], windowRes[1],
            0, GL_BGR, GL_FLOAT, nullptr
        );
        
        // Ensure a 1:1 mapping of the gBuffer's pixels to the framebuffer
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST );
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
        
        glGenerateMipmap( GL_TEXTURE_2D );
        
        // attach the texture to a writable framebuffer
        glFramebufferTexture2D(
            GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i,
            GL_TEXTURE_2D, textures[ i ], 0
        );
    }
    
    // do the same for the depth texture
    glBindRenderbuffer( GL_RENDERBUFFER, renderBuffer[0] );
    glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH32F_STENCIL8, windowRes[0], windowRes[1] );
    glFramebufferRenderbuffer(
        GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBuffer[0]
    );
    
    // setup the final frame texture
    glBindRenderbuffer( GL_RENDERBUFFER, renderBuffer[1] );
    glRenderbufferStorage( GL_RENDERBUFFER, GL_RGBA, windowRes[0], windowRes[1] );
    glFramebufferRenderbuffer(
        GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_RENDERBUFFER, renderBuffer[1]
    );
    
    // ensure the buffers were setup correctly
    GLenum fbStatus = glCheckFramebufferStatus( GL_FRAMEBUFFER );
    if ( fbStatus != GL_FRAMEBUFFER_COMPLETE ) {
        std::cerr
            << "ERROR: Unable to correctly initialize the G-Buffer. "
            << "Please ensure that anOpenGL 3.3 context has been created.\n\t"
            << "Status code " << fbStatus
            << std::endl;
        terminate();
        return false;
    }
    else {
        std::cout
            << "Successfully created an MRT framebuffer.\n\t"
            << "Status:\t" << std::hex << fbStatus << "\n\t"
            << "Id:    \t" << std::dec << fbo
            << std::endl;
    }
    
    // restore the default framebuffer
    glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );
    
    readBuffer = GB_RENDER_BUFFER;
    bufferRes = windowRes;
    
    return true;
}

/******************************************************************************
 * G-BUFFER TERMINATION
******************************************************************************/
void gBuffer::terminate() {
    glDeleteFramebuffers( 1, &fbo );
    fbo = 0;
    
    glDeleteRenderbuffers( 2, renderBuffer );
    renderBuffer[0] = renderBuffer[1] = 0;
    
    glDeleteTextures( 3, textures );
    textures[0] = 0;
    textures[1] = 0;
    textures[2] = 0;
}

/******************************************************************************
 * G-BUFFER GEOMETRY PASS
******************************************************************************/
void gBuffer::bindForGeometryPass() const {
    //glBindFramebuffer( GL_DRAW_FRAMEBUFFER, fbo );
    
    const GLenum drawBuffers[] = {
        GL_COLOR_ATTACHMENT0,
        GL_COLOR_ATTACHMENT1,
        GL_COLOR_ATTACHMENT2,
    };
    
    glDrawBuffers( 3, drawBuffers );
    
#ifdef DEBUG
    //glClear( GL_COLOR_BUFFER_BIT );
#endif
}

/******************************************************************************
 * G-BUFFER STENCIL PASS
******************************************************************************/
void gBuffer::bindForStencilPass() const {
    // disable the draw buffers
    glDrawBuffer( GL_NONE );
}

/******************************************************************************
 * G-BUFFER LIGHTING PASS
******************************************************************************/
void gBuffer::bindForLightPass() const {
    glDrawBuffer( GL_COLOR_ATTACHMENT3 );
    
    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, textures[0] );
    
    glActiveTexture( GL_TEXTURE1 );
    glBindTexture( GL_TEXTURE_2D, textures[1] );
    
    glActiveTexture( GL_TEXTURE2 );
    glBindTexture( GL_TEXTURE_2D, textures[2] );
}

/******************************************************************************
 * G-BUFFER FRAME INITIALIZATION
******************************************************************************/
void gBuffer::bindForWriting() const {
    glBindFramebuffer( GL_DRAW_FRAMEBUFFER, fbo );
    glDrawBuffer( GL_COLOR_ATTACHMENT3 );
}

/******************************************************************************
 * G-BUFFER FRAME ENDING
******************************************************************************/
void gBuffer::bindForReading() const {
    glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );
    glBindFramebuffer( GL_READ_FRAMEBUFFER, fbo );
    glReadBuffer( readBuffer );
}

/******************************************************************************
 * G-BUFFER DEBUG DRAWING
******************************************************************************/
#ifdef DEBUG
void gBuffer::drawBufferDebug() const {
    const vec2i halfRes = bufferRes / 2;
    
    glReadBuffer( GL_COLOR_ATTACHMENT0 );
    glBlitFramebuffer(
        0, 0, bufferRes[0], bufferRes[1],
        0, 0, halfRes[0], halfRes[1],
        GL_COLOR_BUFFER_BIT, GL_NEAREST
    );
    
    glReadBuffer( GL_COLOR_ATTACHMENT1 );
    glBlitFramebuffer(
        0, 0, bufferRes[0], bufferRes[1],
        0, halfRes[1], halfRes[0], bufferRes[1],
        GL_COLOR_BUFFER_BIT, GL_NEAREST
    );
    
    glReadBuffer( GL_COLOR_ATTACHMENT2 );
    glBlitFramebuffer(
        0, 0, bufferRes[0], bufferRes[1],
        halfRes[0], halfRes[1], bufferRes[0], bufferRes[1],
        GL_COLOR_BUFFER_BIT, GL_NEAREST
    );
    
    glReadBuffer( GL_COLOR_ATTACHMENT3 );
    glBlitFramebuffer(
        0, 0, bufferRes[0], bufferRes[1],
        halfRes[0], 0, bufferRes[0], halfRes[1],
        GL_COLOR_BUFFER_BIT, GL_NEAREST
    );
}
#endif

} // end hge namespace
