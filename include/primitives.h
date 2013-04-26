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
        
        void draw() const;
        
        void setVertPos( int index, const vec3& pos );
};

} // end hge namespace

#endif	/* __HGE_PRIMITIVES_H__ */

