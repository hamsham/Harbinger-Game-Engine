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
******************************************************************************/
class primitive {
    public:
        virtual ~primitive() {}
        
        virtual bool init() = 0;
        virtual void terminate() = 0;
};

/******************************************************************************
 * Quad Primitives
******************************************************************************/
class HGE_API quad : public primitive {
    private:
        GLuint vao = 0;
        GLuint vbo = 0;

    public:
        quad    () {}
        ~quad   () { terminate(); }
        
        quad    ( const quad& ) = delete;
        quad    ( quad&& ) = default;
        quad&   operator =  ( const quad& ) = delete;
        quad&   operator =  ( quad&& ) = default;
        
        bool init();
        void terminate();

        void draw() const {
            glBindVertexArray( vao );
            glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
            glBindVertexArray( 0 );
        }
};

/******************************************************************************
 * Triangle Primitives
******************************************************************************/
class HGE_API triangle : public primitive {
    private:
        GLuint vao = 0;
        GLuint vbo = 0;

    public:
        triangle    () {}
        ~triangle   () { terminate(); }
        
        triangle    ( const triangle& ) = delete;
        triangle    ( triangle&& ) = default;
        triangle&   operator =  ( const triangle& ) = delete;
        triangle&   operator =  ( triangle&& ) = default;
        
        bool init();
        void terminate();

        void draw() const {
            glBindVertexArray( vao );
            glDrawArrays( GL_TRIANGLE_STRIP, 0, 3 );
            glBindVertexArray( 0 );
        }
};

/******************************************************************************
 * Line Primitive
******************************************************************************/
class HGE_API line : public primitive {
        
        GLuint  vao = 0;
        GLuint  vbo = 0;
        vec3    points[ 2 ] = { {0.f}, {1.f} };
        
    public:
        line    () {}
        ~line   () { terminate(); }
        
        line    ( const line& ) = delete;
        line    ( line&& ) = default;
        line&   operator =  ( const line& ) = delete;
        line&   operator =  ( line&& ) = default;
        
        bool init();
        void terminate();
        
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
class HGE_API cube : public primitive {
    private:
        GLuint   vao = 0;
        GLuint   vbo = 0;

    public:
        cube    () {}
        ~cube   () { terminate(); }
        
        cube    ( const cube& ) = delete;
        cube    ( cube&& ) = default;
        cube&   operator =  ( const cube& ) = delete;
        cube&   operator =  ( cube&& ) = default;
        
        bool init();
        void terminate();
        
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
class HGE_API sphere : public primitive {
    private:
        GLuint vao = 0;
        GLuint vbo[ 2 ] = { 0, 0 };
        unsigned numIndices = 0;
        
    public:
        sphere      () {}
        ~sphere     () { terminate(); }
        
        sphere      ( const sphere& ) = delete;
        sphere      ( sphere&& ) = default;
        sphere&     operator =  ( const sphere& ) = delete;
        sphere&     operator =  ( sphere&& ) = default;
        
        bool init   ( int rings, int sectors );
        bool init   () { return init( 10, 10 ); }
        void terminate();
        
        void draw() const {
            glBindVertexArray( vao );
            glDrawElements( GL_TRIANGLE_STRIP, numIndices, GL_UNSIGNED_INT, 0 );
            glBindVertexArray( 0 );
        }
};

} // end hge namespace

#endif	/* __HGE_PRIMITIVES_H__ */

