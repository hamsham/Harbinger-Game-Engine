/* 
 * File:   light.h
 * Author: hammy
 *
 * Created on January 1, 2013, 4:37 AM
 */

#ifndef __HGE_LIGHT_H__
#define	__HGE_LIGHT_H__

#include "pipeline.h"

namespace hge {

///////////////////////////////////////////////////////////////////////////////
//		Color Base Structure
///////////////////////////////////////////////////////////////////////////////
struct baseLight {
    vec4    color       = vec4( 1.f );
    float   intensity   = 1.f;
    
    virtual ~baseLight() = 0;
};

inline baseLight::~baseLight() {}

///////////////////////////////////////////////////////////////////////////////
//		Ambient Lighting
///////////////////////////////////////////////////////////////////////////////
struct ambientLight : baseLight {
};

///////////////////////////////////////////////////////////////////////////////
//		Diffuse Lighting
///////////////////////////////////////////////////////////////////////////////
struct diffuseLight : baseLight {
    vec3 dir = vec3( 0.f, 0.f, -1.f );
};

///////////////////////////////////////////////////////////////////////////////
//		Specular Lighting
///////////////////////////////////////////////////////////////////////////////
struct specularLight : baseLight {
    float shininess = 1.f;
};

///////////////////////////////////////////////////////////////////////////////
//		Point Lighting
///////////////////////////////////////////////////////////////////////////////
struct pointLight : baseLight {
    vec3    pos         = vec3( 0.f );
    float   constant    = 1.f;
    float   exponential = 1.f;
    float   linear      = 1.f;
};

///////////////////////////////////////////////////////////////////////////////
//		Spot Lighting
///////////////////////////////////////////////////////////////////////////////
struct spotLight : baseLight {
    vec3    pos         = vec3( 0.f );
    vec3    dir         = vec3( 0.f, 0.f, -1.f );
    float   coneAngle   = HL_PI_INVERSE * 3.15f;
    float   attenuation = 1.f;
};

///////////////////////////////////////////////////////////////////////////////
//		Deferred Point Lighting
///////////////////////////////////////////////////////////////////////////////
struct HGE_API dsPointLight {
    float scale;
    vec3 position;
    vec4 color;
    
    union {
        vec4 attributes;
        struct {
            float constant;
            float exponential;
            float intensity;
            float linear;
        } attrib;
    };
    
    dsPointLight();
    dsPointLight( const dsPointLight& );
    dsPointLight& operator = ( const dsPointLight& );
    ~dsPointLight() {}
    
    float calcInfluenceRadius() const;
};

///////////////////////////////////////////////////////////////////////////////
//		Deferred Point Light Instances
//      To be used with a GBuffer and Deferred Shader
///////////////////////////////////////////////////////////////////////////////
class HGE_API dsLightSphere {
    enum : int {
        DEFAULT_NUM_RINGS   = 10,
        DEFAULT_NUM_SECTORS = 10
    };
    
    enum lightAttributes : int {
        SCALE_ATTRIB    = 1,
        POS_ATTRIB      = 2,
        COLOR_ATTRIB    = 3,
        PARAM_ATTRIB    = 4 // light constant, exponential, intensity, linear
    };
    
    private:
        GLuint vao = 0;
        GLuint vbo[3] = {0,0,0}; // vertex buffer, index buffers, instanced light buffer
        unsigned numIndices = 0;
        unsigned numInstances = 0;
        
    public:
        dsLightSphere   () {}
        dsLightSphere   ( const dsLightSphere& ) = delete;
        dsLightSphere   ( dsLightSphere&& );
        
        ~dsLightSphere  () { terminate(); }
        
        dsLightSphere&  operator =      ( const dsLightSphere& ) = delete;
        dsLightSphere&  operator =      ( dsLightSphere&& );
        
        bool            init            ();
        void            terminate       ();
        
        void            setLightBuffer  ( const dsPointLight*, unsigned lightCount );
        void            setLight        ( unsigned index, const dsPointLight& );
        unsigned        getNumLights    () const { return numInstances; }
        
        void draw() const {
            glBindVertexArray( vao );
            glDrawElementsInstanced( GL_TRIANGLE_STRIP, numIndices, GL_UNSIGNED_INT, 0, numInstances );
            glBindVertexArray( 0 );
        }
};

} // end hge namespace

#endif	/* __HGE_LIGHT_H__ */
