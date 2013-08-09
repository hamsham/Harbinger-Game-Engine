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
    protected:
        GLuint vbo = 0;
        
    public:
        primitive       () {}
        primitive       ( primitive&& );
        primitive       ( const primitive& ) = delete;
        
        virtual ~primitive() = 0;
        
        primitive&      operator =      ( const primitive& ) = delete;
        primitive&      operator =      ( primitive&& );
        
        virtual bool    init            () = 0;
        virtual void    terminate       ();
};

/******************************************************************************
 * Quad Primitives
******************************************************************************/
class HGE_API quad final : public primitive {
    public:
        quad    () { resetDrawMode(); }
        quad    ( const quad& ) = delete;
        quad    ( quad&& ) = default;
        
        ~quad   () { terminate(); }
        
        quad&   operator =      ( const quad& ) = delete;
        quad&   operator =      ( quad&& ) = default;
        
        bool    init            ();

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
    public:
        triangle    () { resetDrawMode(); }
        triangle    ( const triangle& ) = delete;
        triangle    ( triangle&& ) = default;
        
        ~triangle   () { terminate(); }
        
        triangle&   operator =      ( const triangle& ) = delete;
        triangle&   operator =      ( triangle&& ) = default;
        
        bool        init            ();

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
        vec3    points[ 2 ] = { {0.f}, {1.f} };
        
    public:
        line        () { resetDrawMode(); }
        line        ( const line& ) = delete;
        line        ( line&& );
        
        ~line       () { terminate(); }
        
        line&       operator =      ( const line& ) = delete;
        line&       operator =      ( line&& );
        
        bool        init            ();
        void        terminate       () override;
        
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
    public:
        cube    () { resetDrawMode(); }
        cube    ( const cube& ) = delete;
        cube    ( cube&& ) = default;
        
        ~cube   () { terminate(); }
        
        cube&   operator =      ( const cube& ) = delete;
        cube&   operator =      ( cube&& ) = default;
        
        bool    init            ();

        // drawing
        void    resetDrawMode   () { renderMode = pipeline::HGE_TRIANGLE_STRIP; }
        void    draw            () const {
                                    const GLint first[6] = {0,4,8,12,16,20};
                                    const GLsizei count[6] = {4,4,4,4,4,4};
                                    glBindVertexArray( vao );
                                    glMultiDrawArrays( renderMode, first, count, 6 );
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
        GLuint ibo = 0;
        unsigned numIndices = 0;
        
    public:
        sphere      () { resetDrawMode(); }
        sphere      ( const sphere& ) = delete;
        sphere      ( sphere&& );
        
        ~sphere     () { terminate(); }
        
        sphere&     operator =      ( const sphere& ) = delete;
        sphere&     operator =      ( sphere&& );
        
        bool        init            ( int rings, int sectors );
        bool        init            () { return init( 10, 10 ); }
        void        terminate       () override;

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
        int numVerts = 0;
        
    public:
        cone    () { resetDrawMode(); }
        cone    ( const cone& ) = delete;
        cone    ( cone&& );
        
        ~cone   () { terminate(); }
        
        cone&   operator =      ( const cone& ) = delete;
        cone&   operator =      ( cone&& );
        
        bool    init            ( int sectors );
        bool    init            () { return init( 10 ); }
        void    terminate       () override;

        // drawing
        void    resetDrawMode   () { renderMode = pipeline::HGE_TRIANGLE_FAN; }
        void    draw            () const;
};

} // end hge namespace

#endif	/* __HGE_PRIMITIVES_H__ */

