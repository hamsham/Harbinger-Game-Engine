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
#include "resource.h"
#include "bitmap.h"

// Forward Declarations
struct aiScene;
struct aiMaterial;

namespace hge {

///////////////////////////////////////////////////////////////////////////////
//	Mesh Scene & Structure
///////////////////////////////////////////////////////////////////////////////
class HGE_API c_mesh : virtual public c_resource {
    private:
        // Mesh Structure
        struct meshEntry {
            unsigned    matIndex;
            unsigned    baseVertex;
            unsigned    baseIndex;
            unsigned    numIndices;

            meshEntry           ();
            meshEntry           ( const meshEntry& );
            ~meshEntry          () {}

            meshEntry& operator= ( const meshEntry& );
        };

        //Private Members
        unsigned                numMeshes       = 0;
        unsigned                numTextures     = 0;
        GLuint                  vao             = 0u;
        GLuint                  buffers[ 2 ]    = { 0u, 0u }; // Vertices & Indices
        meshEntry*              entries         = nullptr;
        c_bitmap*               textures        = nullptr;

        //load all data from Assimp using two passes
        bool	prepMeshes      ( const aiScene*, unsigned& numVerts, unsigned& numIndices );
        bool	loadMeshes      ( const aiScene*, s_bumpVertex* vertArray, unsigned* indexArray );
        bool	loadTextures    ( const aiScene*, const char* fileName );
        void	loadTexType     ( int index, const aiMaterial*, int textureType, const std::string& directory );
        void	loadVao         ( s_bumpVertex* vertices, unsigned numVertices, unsigned* indices, unsigned numIndices );

    public:
        c_mesh	() {}
        c_mesh  ( c_mesh&& );
        c_mesh  ( const c_mesh& ) = delete;
        ~c_mesh	() { unload(); }
        
        c_mesh& operator =      ( c_mesh&& );
        c_mesh& operator =      ( const c_mesh& ) = delete;
        bool    operator ==     ( const c_mesh& m ) { return vao == m.vao; }
        bool    operator !=     ( const c_mesh& m ) { return vao != m.vao; }

        //data loading and unloading
        bool	isLoaded		() const;
        bool	load			( const char* fileName, int flags = 0 );
        void	unload			();

        // drawing
        void	draw			() const;
};

} // end hge namespace

#endif	/* __HGE_MESH_H__ */
