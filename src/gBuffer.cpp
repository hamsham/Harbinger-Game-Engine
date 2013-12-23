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
    fbo{ gb.fbo },
    renderBuffer{ gb.renderBuffer },
    textures{ gb.textures[0], gb.textures[1], gb.textures[2] },
    bufferRes( gb.bufferRes )
{
    gb.readBuffer = GB_RENDER_BUFFER;
    gb.fbo = 0;
    gb.renderBuffer = 0;
    gb.textures[0] = gb.textures[1] = gb.textures[2] = 0;
    gb.bufferRes = vec2i( 0,0 );
}

gBuffer& gBuffer::operator =( gBuffer&& gb ) {
    terminate();
    readBuffer = gb.readBuffer;
    gb.readBuffer = GB_RENDER_BUFFER;
    
    fbo = gb.fbo;
    gb.fbo = 0;
    
    renderBuffer = gb.renderBuffer;
    gb.renderBuffer = 0;
    
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
        glGenRenderbuffers( 1, &renderBuffer );
        glGenTextures( 3, textures );

        glBindFramebuffer( GL_DRAW_FRAMEBUFFER, fbo );

        if ( !fbo || !renderBuffer ) {
            terminate();
            std::cerr << "ERROR: Unable to create storage for the G-Buffer." << std::endl;
            return false;
        }
    }
    
    // initialize the depth buffer
    glBindRenderbuffer( GL_RENDERBUFFER, renderBuffer );
    glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, windowRes[0], windowRes[1] );
    glFramebufferRenderbuffer(
        GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderBuffer
    );
    
    /*
     * Positions buffer
     */
    glBindTexture( GL_TEXTURE_RECTANGLE, textures[ 0 ] );
    glTexImage2D( GL_TEXTURE_RECTANGLE, 0, GL_RGBA32F, windowRes[0], windowRes[1], 0, GL_BGRA, GL_FLOAT, nullptr );
    glTexParameterf( GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameterf( GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    glTexParameterf( GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    
    /*
     * Color buffer (uses RGB8 unsigned bytes)
     */
    glBindTexture( GL_TEXTURE_RECTANGLE, textures[ 1 ] );
    glTexImage2D( GL_TEXTURE_RECTANGLE, 0, GL_RGBA, windowRes[0], windowRes[1], 0, GL_BGRA, GL_UNSIGNED_BYTE, nullptr );
    glTexParameterf( GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameterf( GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    glTexParameterf( GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    
    /*
     * Normals buffer
     */
    glBindTexture( GL_TEXTURE_RECTANGLE, textures[ 2 ] );
    glTexImage2D( GL_TEXTURE_RECTANGLE, 0, GL_RGBA16F, windowRes[0], windowRes[1], 0, GL_BGRA, GL_FLOAT, nullptr );
    glTexParameterf( GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameterf( GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    glTexParameterf( GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    
    glFramebufferTexture2D( GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_RECTANGLE, textures[ 0 ], 0 );
    glFramebufferTexture2D( GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_RECTANGLE, textures[ 1 ], 0 );
    glFramebufferTexture2D( GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_RECTANGLE, textures[ 2 ], 0 );
    
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
    glBindRenderbuffer( GL_RENDERBUFFER, 0 );
    
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
    
    glDeleteRenderbuffers( 1, &renderBuffer );
    renderBuffer = 0;
    
    glDeleteTextures( 3, textures );
    textures[0] = 0;
    textures[1] = 0;
    textures[2] = 0;
}

/******************************************************************************
 * G-BUFFER GEOMETRY PASS
******************************************************************************/
void gBuffer::bindForGeometryPass() const {
    glBindFramebuffer( GL_FRAMEBUFFER, fbo );
    
    const GLenum drawBuffers[] = {
        GL_COLOR_ATTACHMENT0,
        GL_COLOR_ATTACHMENT1,
        GL_COLOR_ATTACHMENT2
    };
    
    glDrawBuffers( 3, drawBuffers );
}

/******************************************************************************
 * G-BUFFER LIGHTING PASS
******************************************************************************/
void gBuffer::bindForLightPass() const {
    glDrawBuffer( 0 );
    
    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_RECTANGLE, textures[0] );
    
    glActiveTexture( GL_TEXTURE1 );
    glBindTexture( GL_TEXTURE_RECTANGLE, textures[1] );
    
    glActiveTexture( GL_TEXTURE2 );
    glBindTexture( GL_TEXTURE_RECTANGLE, textures[2] );
}

/******************************************************************************
 * G-BUFFER FRAME ENDING
******************************************************************************/
void gBuffer::unbind() const {
    glBindFramebuffer( GL_FRAMEBUFFER, 0 );
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
