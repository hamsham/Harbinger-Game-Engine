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
#include "drawable.h"

namespace hge {

/******************************************************************************
 * Primitive Setup
******************************************************************************/
class HGE_API primitive : public drawable {
    public:
        primitive       () {}
        primitive       ( primitive&& );
        primitive       ( const primitive& ) = delete;
        
        virtual ~primitive() {}
        
        primitive&      operator =      ( const primitive& ) = delete;
        primitive&      operator =      ( primitive&& );
        
        virtual bool    init            () = 0;
        virtual void    terminate       () = 0;
};

/******************************************************************************
 * Quad Primitives
******************************************************************************/
class HGE_API quad final : public primitive {
    private:
        GLuint vbo = 0;

    public:
        quad    () {}
        quad    ( const quad& ) = delete;
        quad    ( quad&& );
        
        ~quad   () { terminate(); }
        
        quad&   operator =      ( const quad& ) = delete;
        quad&   operator =      ( quad&& );
        
        bool    init            ();
        void    terminate       ();

        // drawing
        void    resetDrawMode   () { renderMode = pipeline::HGE_TRIANGLE_STRIP; }
        void    draw            () const {
                                    glBindVertexArray( vao );
                                    glDrawArrays( renderMode, 0, 4 );
                                    glBindVertexArray( 0 );
                                }
};

/******************************************************************************
 * Triangle Primitives
******************************************************************************/
class HGE_API triangle final : public primitive {
    private:
        GLuint vbo = 0;

    public:
        triangle    () {}
        triangle    ( const triangle& ) = delete;
        triangle    ( triangle&& );
        
        ~triangle   () { terminate(); }
        
        triangle&   operator =      ( const triangle& ) = delete;
        triangle&   operator =      ( triangle&& );
        
        bool        init            ();
        void        terminate       ();

        // drawing
        void        resetDrawMode   () { renderMode = pipeline::HGE_TRIANGLES; }
        void        draw            () const {
                                        glBindVertexArray( vao );
                                        glDrawArrays( renderMode, 0, 3 );
                                        glBindVertexArray( 0 );
                                    }
};

/******************************************************************************
 * Line Primitive
******************************************************************************/
class HGE_API line final : public primitive {
        GLuint  vbo = 0;
        vec3    points[ 2 ] = { {0.f}, {1.f} };
        
    public:
        line        () {}
        line        ( const line& ) = delete;
        line        ( line&& );
        
        ~line       () { terminate(); }
        
        line&       operator =      ( const line& ) = delete;
        line&       operator =      ( line&& );
        
        bool        init            ();
        void        terminate       ();
        
        // attribute manipulation
        void        enableAttribute ( pipeline::attribute ) override {}
        void        disableAttribute( pipeline::attribute ) override {}
        
        void        setVertPos      ( int index, const vec3& inPos );

        // drawing
        void        resetDrawMode   () { renderMode = pipeline::HGE_LINES; }
        void        draw            () const {
                                        glBindVertexArray( vao );
                                        glDrawArrays( renderMode, 0, 2 );
                                        glBindVertexArray( 0 );
                                    }
};

/******************************************************************************
 * Cube Primitive
 * 
 * NOTE:
 *      Cubes can also be rendered using a cubemap texture
******************************************************************************/
class HGE_API cube final : public primitive {
    private:
        GLuint   vbo = 0;

    public:
        cube    () {}
        cube    ( const cube& ) = delete;
        cube    ( cube&& );
        
        ~cube   () { terminate(); }
        
        cube&   operator =      ( const cube& ) = delete;
        cube&   operator =      ( cube&& );
        
        bool    init            ();
        void    terminate       ();

        // drawing
        void    resetDrawMode   () { renderMode = pipeline::HGE_TRIANGLE_STRIP; }
        void    draw            () const {
                                    glBindVertexArray( vao );
                                    glDrawArrays( renderMode, 0, 26 );
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
class HGE_API sphere final : public primitive {
    private:
        GLuint vbo[ 2 ] = { 0, 0 };
        unsigned numIndices = 0;
        
    public:
        sphere      () {}
        sphere      ( const sphere& ) = delete;
        sphere      ( sphere&& );
        
        ~sphere     () { terminate(); }
        
        sphere&     operator =      ( const sphere& ) = delete;
        sphere&     operator =      ( sphere&& );
        
        bool        init            ( int rings, int sectors );
        bool        init            () { return init( 10, 10 ); }
        void        terminate       ();

        // drawing
        void        resetDrawMode   () { renderMode = pipeline::HGE_TRIANGLE_STRIP; }
        void        draw            () const {
                                        glBindVertexArray( vao );
                                        glDrawElements(
                                            renderMode, numIndices,
                                            GL_UNSIGNED_INT, 0
                                        );
                                        glBindVertexArray( 0 );
                                    }
};

/******************************************************************************
 * Cone Primitive
******************************************************************************/
class HGE_API cone final : public primitive {
    private:
        GLuint vbo = 0;
        int numVerts = 0;
        
    public:
        cone    () {}
        cone    ( const cone& ) = delete;
        cone    ( cone&& );
        
        ~cone   () { terminate(); }
        
        cone&   operator =      ( const cone& ) = delete;
        cone&   operator =      ( cone&& );
        
        bool    init            ( int sectors );
        bool    init            () { return init( 10 ); }
        void    terminate       ();

        // drawing
        void    resetDrawMode   () { renderMode = pipeline::HGE_TRIANGLE_FAN; }
        void    draw            () const;
};

} // end hge namespace

#endif	/* __HGE_PRIMITIVES_H__ */

