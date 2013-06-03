/* 
 * File:   primitives.h
 * Author: hammy
 *
 * Created on April 22, 2013, 8:40 PM
 */

#ifndef __HGE_PRIMITIVES_H__
#define	__HGE_PRIMITIVES_H__

#include "mesh.h"
#include "object.h"
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
class HGE_API c_quad : virtual public c_drawableObj {
    private:
        static GLuint   vao;
        static GLuint   vbo;
        static s_vertex verts[ 4 ];

    public:
        c_quad  () {}
        ~c_quad () {}

        static bool init();
        static void terminate();

        void draw() const {
            glBindVertexArray( vao );
            glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
            glBindVertexArray( 0 );
    }
};

/******************************************************************************
 * Triangle Primitives
******************************************************************************/
class HGE_API c_triangle : virtual public c_drawableObj {
    private:
        static GLuint   vao;
        static GLuint   vbo;
        static s_vertex verts[ 3 ];

    public:
        c_triangle  () {}
        ~c_triangle () {}

        static bool init();
        static void terminate();

        void draw() const {
            glBindVertexArray( vao );
            glDrawArrays( GL_TRIANGLE_STRIP, 0, 3 );
            glBindVertexArray( 0 );
    }
};

/******************************************************************************
 * Line Primitive
******************************************************************************/
class HGE_API c_line : virtual public c_drawableObj {
    private:
        static GLuint vao;
        static GLuint vbo;
        
        vec3 points[ 2 ] = { {0.f}, {1.f} };
        
    public:
        c_line() {}
        ~c_line() {}
        
        static bool init();
        static void terminate();
        
        void setVertPos( int index, const vec3& pos );
        
        void draw() const {
            glBindVertexArray( vao );
            glDrawArrays( GL_LINES, 0, 2 );
            glBindVertexArray( 0 );
        }
};


/******************************************************************************
 * Sphere Primitive
 * 
 * NOTE:
 *      Use c_drawableObj::scale as the sphere's radius
 *      Spheres can also be rendered using a cubemap texture
******************************************************************************/
class HGE_API c_sphere : virtual public c_drawableObj {
    private:
        GLuint vao = 0;
        GLuint vbo[ 2 ] = { 0, 0 };
        
        s_vertex* vertices = nullptr;
        unsigned numVerts = 0;
        
        GLuint* indices = nullptr;
        unsigned numIndices = 0;
        
        bool sendToOpenGL();
        
    public:
        c_sphere() {}
        ~c_sphere();
        
        bool createSphere( int rings, int sectors );
        void destroySphere();
        
        void draw() const {
            glBindVertexArray( vao );
            glDrawElements( GL_TRIANGLE_STRIP, numIndices, GL_UNSIGNED_INT, 0 );
            glBindVertexArray( 0 );
        }
};

} // end hge namespace

#endif	/* __HGE_PRIMITIVES_H__ */

