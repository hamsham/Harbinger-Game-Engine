/* 
 * File:   primitives.h
 * Author: hammy
 *
 * Created on April 22, 2013, 8:40 PM
 */

#ifndef __HGE_PRIMITIVES_H__
#define	__HGE_PRIMITIVES_H__

#include "types.h"
#include "pipeline.h"

namespace hge {

/******************************************************************************
 * Primitive Setup
 * These functions are called automatically from the graphics pipeline
******************************************************************************/
HGE_API bool initPrimitives();
HGE_API void terminatePrimitives();

/******************************************************************************
 * Quad Primitives
******************************************************************************/
class HGE_API quad {
    private:
        friend HGE_API bool initPrimitives();
        friend HGE_API void terminatePrimitives();

        static bool init();
        static void terminate();
        
        static GLuint   vao;
        static GLuint   vbo;

    public:
        quad    () {}
        ~quad   () {}
        
        quad    ( const quad& ) = delete;
        quad    ( quad&& ) = default;
        quad&   operator =  ( const quad& ) = delete;
        quad&   operator =  ( quad&& ) = default;

        void draw() const {
            glBindVertexArray( vao );
            glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
            glBindVertexArray( 0 );
        }
};

/******************************************************************************
 * Triangle Primitives
******************************************************************************/
class HGE_API triangle {
    private:
        friend HGE_API bool initPrimitives();
        friend HGE_API void terminatePrimitives();

        static bool init();
        static void terminate();
        
        static GLuint   vao;
        static GLuint   vbo;

    public:
        triangle    () {}
        ~triangle   () {}
        
        triangle    ( const triangle& ) = delete;
        triangle    ( triangle&& ) = default;
        triangle&   operator =  ( const triangle& ) = delete;
        triangle&   operator =  ( triangle&& ) = default;

        void draw() const {
            glBindVertexArray( vao );
            glDrawArrays( GL_TRIANGLE_STRIP, 0, 3 );
            glBindVertexArray( 0 );
        }
};

/******************************************************************************
 * Line Primitive
******************************************************************************/
class HGE_API line {
    private:
        friend HGE_API bool initPrimitives();
        friend HGE_API void terminatePrimitives();

        static bool init();
        static void terminate();
        
        static GLuint   vao;
        static GLuint   vbo;

        vec3 points[ 2 ] = { {0.f}, {1.f} };
        
    public:
        line    () {}
        ~line   () {}
        
        line    ( const line& ) = delete;
        line    ( line&& ) = default;
        line&   operator =  ( const line& ) = delete;
        line&   operator =  ( line&& ) = default;
        
        void setVertPos( int index, const vec3& inPos );
        
        void draw() const {
            glBindVertexArray( vao );
            glDrawArrays( GL_LINES, 0, 2 );
            glBindVertexArray( 0 );
        }
};

/******************************************************************************
 * Cube Primitive
 * 
 * NOTE:
 *      Cubes can also be rendered using a cubemap texture
******************************************************************************/
class HGE_API cube {
    private:
        friend HGE_API bool initPrimitives();
        friend HGE_API void terminatePrimitives();

        static bool init();
        static void terminate();
        
        static GLuint   vao;
        static GLuint   vbo;

    public:
        cube    () {}
        ~cube   () {}
        
        cube    ( const cube& ) = delete;
        cube    ( cube&& ) = default;
        cube&   operator =  ( const cube& ) = delete;
        cube&   operator =  ( cube&& ) = default;
        
        void draw() const {
            glBindVertexArray( vao );
            glDrawArrays( GL_TRIANGLE_STRIP, 0, 26 );
            glBindVertexArray( 0 );
        }
};

/******************************************************************************
 * Sphere Primitive
 * 
 * NOTE:
 *      Use c_drawable::scale as the sphere's radius
 *      Spheres can also be rendered using a cubemap texture
******************************************************************************/
class HGE_API sphere {
    private:
        GLuint vao = 0;
        GLuint vbo[ 2 ] = { 0, 0 };
        
        bumpVertex* vertices = nullptr;
        unsigned numVerts = 0;
        
        GLuint* indices = nullptr;
        unsigned numIndices = 0;
        
        bool sendToOpenGL();
        
    public:
        sphere      () {}
        ~sphere     () { destroySphere(); }
        
        sphere      ( const sphere& ) = delete;
        sphere      ( sphere&& ) = default;
        sphere&     operator =  ( const sphere& ) = delete;
        sphere&     operator =  ( sphere&& ) = default;
        
        bool createSphere       ( int rings, int sectors );
        void destroySphere      ();
        
        void draw() const {
            glBindVertexArray( vao );
            glDrawElements( GL_TRIANGLE_STRIP, numIndices, GL_UNSIGNED_INT, 0 );
            glBindVertexArray( 0 );
        }
};

} // end hge namespace

#endif	/* __HGE_PRIMITIVES_H__ */

