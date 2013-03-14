/* 
 * File:   mesh.h
 * Author: hammy
 *
 * Created on December 30, 2012, 6:00 PM
 */

#ifndef __HGE_MESH_H__
#define	__HGE_MESH_H__

#include <GL/glew.h>
#include <assimp/scene.h>

#include "types.h"
#include "resource.h"
#include "object.h"

///////////////////////////////////////////////////////////////////////////////
//	Mesh Verices
///////////////////////////////////////////////////////////////////////////////
struct HGE_API s_vertex {
	vec3 pos;
	vec2 uv;
	vec3 norm;
	vec3 tangent;
	
	s_vertex			();
	s_vertex			( const s_vertex& );
	void setPos			( float x, float y, float z );
	void setUVs			( float u, float v );
	void setNorm		( float x, float y, float z );
	void setTangent		( float x, float y, float z );
};

///////////////////////////////////////////////////////////////////////////////
//	Mesh Scene & Structure
///////////////////////////////////////////////////////////////////////////////
class HGE_API c_mesh : virtual public c_resource, virtual public c_drawableObj {
	public:
		enum e_drawMode {
			DRAW_FILLED	= GL_TRIANGLES,
			DRAW_LINES	= GL_LINES,
			DRAW_POINTS	= GL_POINTS
		};
		static const int		DEFAULT_MESH_FLAGS;
	
	private:
		// Mesh Structure
		struct meshEntry {
			uint	matIndex;
			uint	baseVertex;
			uint	baseIndex;
			uint	numIndices;

			meshEntry			();
			meshEntry			( const meshEntry& );
			~meshEntry			() {}

			meshEntry& operator= ( const meshEntry& );
		};

		//Private Members
		uint					numMeshes;
		uint					numTextures;
		GLuint					vao;
		GLuint					buffers[ 2 ]; // Vertices & Indices
		meshEntry*				entries;
		c_bitmap*				textures;
		e_drawMode				drawMode;
		
		//load all data from Assimp using two passes
		bool	prepMeshes		( const aiScene*, uint& numVerts, uint& numIndices );
		bool	loadMeshes		( const aiScene*, s_vertex* vertArray, uint* indexArray );
		bool	loadTextures	( const aiScene*, cstr fileName );
		bool	loadTexType		( aiMaterial*, aiTextureType, const std::string& directory, uint& texIter );
		void	loadVao			( s_vertex* vertices, uint numVertices, uint* indices, uint numIndices );

	public:
		c_mesh	();
		~c_mesh	();
		
		//deleted member functions
		c_mesh	( const c_mesh& )	= delete;
		c_mesh&	operator =		( const c_mesh& ) = delete;
		bool	operator ==		( const c_mesh& ) = delete;
		bool	operator !=		( const c_mesh& ) = delete;
		
		//data loading and unloading
		bool	isLoaded		() const;
		bool	load			( cstr fileName, int flags = DEFAULT_MESH_FLAGS );
		void	unload			();
		
		// render modifications
		void	setDrawMode		( e_drawMode mode )				{ drawMode = mode; }
		e_drawMode getDrawMode	() const						{ return drawMode; }
		
		// drawing
		void	tick			( float timeElapsed )			{ c_drawableObj::tick( timeElapsed ); }
		void	draw			() const;
		void	draw			( const c_camera& ) const;
};

#endif	/* __HGE_MESH_H__ */
