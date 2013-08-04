/* 
 * File:   mesh.h
 * Author: hammy
 *
 * Created on December 30, 2012, 6:00 PM
 */

#ifndef __HGE_MESH_H__
#define	__HGE_MESH_H__

#include <string>
#include "types.h"
#include "pipeline.h"
#include "drawable.h"
#include "resource.h"
#include "bitmap.h"

// Forward Declarations
struct aiScene;
struct aiMaterial;

namespace hge {

/******************************************************************************
//	Mesh Scene & Structure
******************************************************************************/
class HGE_API mesh final : public resource, public drawable {
    private:
        // Mesh Structure
        struct meshEntry {
            unsigned    matIndex    = 0;
            unsigned    baseVertex  = 0;
            unsigned    baseIndex   = 0;
            unsigned    numIndices  = 0;

//            meshEntry           ();
//            meshEntry           ( const meshEntry& );
//            ~meshEntry          () {}
//
//            meshEntry& operator= ( const meshEntry& );
        };

        //Private Members
        unsigned                numMeshes       = 0;
        unsigned                numTextures     = 0;
        GLuint                  buffers[ 2 ]    = { 0u, 0u }; // Vertices & Indices
        meshEntry*              entries         = nullptr;
        bitmap*                 textures        = nullptr;

        //load all data from Assimp using two passes
        bool	prepMeshes      ( const aiScene*, unsigned& numVerts, unsigned& numIndices );
        bool	loadMeshes      ( const aiScene*, bumpVertex* vertArray, unsigned* indexArray );
        bool	loadTextures    ( const aiScene*, const char* fileName );
        void	loadTexType     ( int index, const aiMaterial*, int textureType, const std::string& directory );
        void	loadVao         ( bumpVertex* vertices, unsigned numVertices, unsigned* indices, unsigned numIndices );

    public:
        mesh	() {}
        mesh    ( mesh&& );
        mesh    ( const mesh& ) = delete;
        ~mesh	() { unload(); }
        
        mesh&   operator =      ( mesh&& );
        mesh&   operator =      ( const mesh& ) = delete;
        bool    operator ==     ( const mesh& m ) { return vao == m.vao; }
        bool    operator !=     ( const mesh& m ) { return vao != m.vao; }

        //data loading and unloading
        bool	isLoaded		() const;
        bool	load			( const char* fileName, int flags = 0 );
        void	unload			();

        // drawing
        void    resetDrawMode   () { renderMode = GL_TRIANGLES; }
        void	draw			() const;
};

} // end hge namespace

#endif	/* __HGE_MESH_H__ */
