/* 
 * File:   mesh.cpp
 * Author: hammy
 * 
 * Created on December 30, 2012, 6:00 PM
 */

#include <iostream>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "types.h"
#include "pipeline.h"
#include "bitmap.h"
#include "mesh.h"

namespace hge {

///////////////////////////////////////////////////////////////////////////////
//	Mesh Verices
///////////////////////////////////////////////////////////////////////////////
s_vertex::s_vertex() :
	pos(),
	uv(),
	norm(),
	tangent()
{}

s_vertex::s_vertex( const s_vertex& v ) :
	pos( v.pos ),
	uv( v.uv ),
	norm( v.norm ),
	tangent( v.tangent )
{}

void s_vertex::setPos( float x, float y, float z ) {
	pos[ 0 ] = x;
	pos[ 1 ] = y;
	pos[ 2 ] = z;
}

void s_vertex::setUVs( float u, float v ) {
	uv[ 0 ] = u;
	uv[ 1 ] = v;
}

void s_vertex::setNorm( float x, float y, float z ) {
	norm[ 0 ] = x;
	norm[ 1 ] = y;
	norm[ 2 ] = z;
}

void s_vertex::setTangent( float x, float y, float z ) {
	tangent[ 0 ] = x;
	tangent[ 1 ] = y;
	tangent[ 2 ] = z;
}

///////////////////////////////////////////////////////////////////////////////
//	Mesh Structure
///////////////////////////////////////////////////////////////////////////////
c_mesh::meshEntry::meshEntry() :
	matIndex	( 0 ),
	baseVertex	( 0 ),
	baseIndex	( 0 ),
	numIndices	( 0 )
{}

c_mesh::meshEntry::meshEntry( const c_mesh::meshEntry& meCopy) :
	matIndex	( meCopy.matIndex ),
	baseVertex	( meCopy.baseVertex ),
	baseIndex	( meCopy.baseIndex ),
	numIndices	( meCopy.numIndices )
{}

c_mesh::meshEntry& c_mesh::meshEntry::operator = ( const c_mesh::meshEntry& meCopy ) {
	matIndex	= meCopy.matIndex;
	baseVertex	= meCopy.baseVertex;
	baseIndex	= meCopy.baseIndex;
	numIndices	= meCopy.numIndices;
	return *this;
}

///////////////////////////////////////////////////////////////////////////////
//	Mesh Resources
///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
// Mesh - Unloading
//-----------------------------------------------------------------------------
void c_mesh::unload() {
	if ( entries ) {
		delete [] entries;
		entries = HGE_NULL;
		numMeshes = 0;
	}
	if ( textures ) {
		delete [] textures;
		textures = HGE_NULL;
		numTextures = 0;
	}
    
	glDeleteVertexArrays( 1, &vao );
	vao = 0;
    
	glDeleteBuffers( 2, buffers );
    buffers[ 0 ] = buffers[ 1 ] = 0;
}

//-----------------------------------------------------------------------------
// Mesh - Loading Check
//-----------------------------------------------------------------------------
bool c_mesh::isLoaded() const {
	return ( entries != HGE_NULL && textures != HGE_NULL );
}

//-----------------------------------------------------------------------------
// Mesh - Loading
//-----------------------------------------------------------------------------
bool c_mesh::load( cstr fileName, int flags ) {
	std::cout << "Loading 3d mesh data:" << "\n\tFile:\t" << fileName << "\n";
	
	unload();
	Assimp::Importer importer;
	const aiScene* pScene( HGE_NULL );
	s_vertex* vertArray( HGE_NULL );
	uint* indexArray( HGE_NULL );
	uint numVerts( 0 );
	uint numIndices( 0 );
	
	glGenVertexArrays( 1, &vao );
	glBindVertexArray( vao );
	
	std::cout << "\tReading data from the file...\n";
	pScene = importer.ReadFile( fileName, aiProcessPreset_TargetRealtime_MaxQuality );
	if ( pScene == HGE_NULL ) {
		std::cout << "\tFailed. " << importer.GetErrorString() << std::endl;
		return false;
	}
	std::cout << "\tDone.\n";
	
	// Prepare data from the file
	std::cout << "\tPreparing mesh data...\n";
	if ( !prepMeshes( pScene, numVerts, numIndices ) ) {
		std::cout << "\tFailed. Aborting file import." << std::endl;
		return false;
	}
	vertArray = new s_vertex[ numVerts ];
	indexArray = new uint[ numIndices ];
	entries = new meshEntry[ numMeshes ];
	std::cout << "\tDone\n";
	
	// convert Assimp's meshes to OpenGL-compatible arrays
	std::cout << "\tLoading mesh data...\n";
	if ( !loadMeshes( pScene, vertArray, indexArray ) ) {
		std::cout << "\tFailed. Aborting file import." << std::endl;
		delete [] vertArray;
		delete [] indexArray;
		unload();
		return false;
	}
	std::cout << "\tDone\n";
	
	// load all material data from the file
	std::cout << "\tLoading texture & material data...\n";
    numTextures = pScene->mNumMaterials;
	textures = new c_bitmap [ numTextures ];
	if ( !loadTextures( pScene, fileName ) ) {
		std::cout << "\t\tWarning: Unable to load all associated materials.\n";
		delete [] textures;
		textures = HGE_NULL;
		numTextures = 0;
	}
	std::cout << "\tDone.\n";
	
	std::cout << "\tSending mesh data to the GPU...\n";
	loadVao( vertArray, numVerts, indexArray, numIndices );
	std::cout << "\tDone.\n";
	delete [] vertArray;
	delete [] indexArray;
	
	glBindVertexArray( 0 );
	
	std::cout << "Successfully loaded " << fileName << std::endl;
	return true;
}

//-----------------------------------------------------------------------------
// Mesh - Data preparation process
//-----------------------------------------------------------------------------
bool c_mesh::prepMeshes( const aiScene* pScene, uint& numVerts, uint& numIndices ) {
	const aiMesh* pMesh( HGE_NULL );
	numVerts = 0;
	numIndices = 0;
	numMeshes = pScene->mNumMeshes;
	
	for ( uint i( 0 ); i < numMeshes; ++i ) {
		pMesh = pScene->mMeshes[ i ];
		
		for ( uint f( 0 ); f < pMesh->mNumFaces; ++f ) {
			if ( pMesh->mFaces[ f ].mNumIndices != 3 ) {
				std::cout << "\tError: Mesh contains non-triangulated faces\n";
				numVerts = 0;
				numIndices = 0;
				return false;
			}
			numIndices += pMesh->mFaces[ f ].mNumIndices;
		}
		
		for ( uint j( 0 ); j < pMesh->mNumVertices; ++j ) {
			++numVerts;
		}
	}
	std::cout
		<< "\t\t" << numVerts << " vertices\n"
		<< "\t\t" << numIndices << " indices\n"
		<< "\t\t" << numMeshes << " meshes\n";
	return true;
}

//-----------------------------------------------------------------------------
// Mesh - Mesh data loading
//-----------------------------------------------------------------------------
bool c_mesh::loadMeshes( const aiScene* pScene, s_vertex* vertArray, uint* indexArray ) {
	const aiMesh* pMesh( HGE_NULL );
    uint vertIter( 0 );
    uint indexIter( 0 );
    uint sumVerts( 0 );
    uint sumIndices( 0 );

    for ( uint i( 0 ); i < numMeshes; ++i ) {
        pMesh = pScene->mMeshes[ i ];
        std::cout << "\t\tMaterial " << i << " uses texture " << pMesh->mMaterialIndex << ".\n";

        // prep the data for each sub-mesh
        entries[ i ].matIndex	= pMesh->mMaterialIndex;
        entries[ i ].numIndices	= pMesh->mNumFaces * 3;
        entries[ i ].baseVertex	= sumVerts;
        entries[ i ].baseIndex	= sumIndices;
        sumVerts				+= pMesh->mNumVertices;
        sumIndices				+= entries[ i ].numIndices;

        //get all texture coordinates, normals, and vertex positions
        for ( uint j( 0 ); j < pMesh->mNumVertices; ++j ) {
            const aiVector3D* pPos          = &pMesh->mVertices[ j ];
            const aiVector3D* pNorm         = &pMesh->mNormals[ j ];
            vertArray[ vertIter ].setPos    ( pPos->x, pPos->y, pPos->z );
            vertArray[ vertIter ].setNorm   ( pNorm->x, pNorm->y, pNorm->z );
            
            if ( pMesh->HasTangentsAndBitangents() ) {
                const aiVector3D* pTangent = &pMesh->mTangents[ j ];
                vertArray[ vertIter ].setTangent( pTangent->x, pTangent->y, pTangent->z );
            }

            if ( pMesh->HasTextureCoords( 0 ) ) {
                vertArray[ vertIter ].uv[ 0 ] = pMesh->mTextureCoords[ 0 ][ j ].x;
                vertArray[ vertIter ].uv[ 1 ] = pMesh->mTextureCoords[ 0 ][ j ].y;
            }
            ++vertIter;
        }

        // gather all indices from the meshEntry faces
        for ( uint f( 0 ); f < pMesh->mNumFaces; ++f ) {
            const aiFace* face          = &pMesh->mFaces[ f ];
            indexArray[ indexIter++ ]   = face->mIndices[ 0 ];
            indexArray[ indexIter++ ]   = face->mIndices[ 1 ];
            indexArray[ indexIter++ ]   = face->mIndices[ 2 ];
        }
        std::cout << "\t\tLoaded sub-mesh " << i << ".\n";
    }

    return true;
}

//-----------------------------------------------------------------------------
// Mesh - Texture Loading
//-----------------------------------------------------------------------------
bool c_mesh::loadTextures( const aiScene* pScene, cstr fileName ) {
    std::string currDir = fileName;
    std::string::size_type slash = currDir.find_last_of("/\\");

    // ensure the correct folder for the materials
    if ( slash == std::string::npos ) {
        currDir = '.';
    }
    else if ( slash == 0 ) {
        currDir = '/';
    }
    else {
        currDir.erase( slash );
    }

    //begin loading materials
    std::cout << "\t\t# Textures: " << numTextures << "\n";

    //get a list of all the textures associated with each meshEntry
    for ( uint i( 0 ); i < numTextures; ++i ) {

        const aiMaterial* pMaterial = pScene->mMaterials[ i ];

        if ( pMaterial->GetTextureCount( aiTextureType_DIFFUSE ) > 0 )
            loadTexType( i, pMaterial, aiTextureType_DIFFUSE, currDir );

    }

    return true;
}

//-----------------------------------------------------------------------------
// Mesh - Loading Specific Texture Types
//-----------------------------------------------------------------------------
void c_mesh::loadTexType(
    int index,
    const aiMaterial* pMaterial,
    int textureType,
    const std::string& directory
) {
    aiString path;
    std::string fullPath;
    if ( pMaterial->GetTexture( (aiTextureType)textureType, 0, &path ) == AI_SUCCESS ) {
        if ( path.data[0] != '/' )
            fullPath = directory + '/' + path.data;
        else
            fullPath = directory + path.data;
        if ( !textures[ index ].load( fullPath.c_str() ) ) {
            std::cerr << "Unable to load " << fullPath << '\n';
        }
        else {
            std::cout << "Successfully loaded " << fullPath << '\n';
            textures[ index ].setTexUnit( pipeline::HGE_TEXTURE_DEFAULT + (textureType-1));
        }
    }
}

//-----------------------------------------------------------------------------
// Mesh - Send data to the GPU
//-----------------------------------------------------------------------------
void c_mesh::loadVao(
	s_vertex* vertices, uint numVertices,
	uint* indices, uint numIndices
) {
	// create a buffer for the vertex positions & indices
	glGenBuffers( 2, buffers );
	
	glBindBuffer( GL_ARRAY_BUFFER, buffers[ 0 ] );
	glBufferData( GL_ARRAY_BUFFER, sizeof( s_vertex ) * numVertices, vertices, GL_STATIC_DRAW );
	printGlError( "Error while sending mesh data to the GPU.");
	
	//send the vertices to opengl
	glEnableVertexAttribArray( pipeline::VERTEX_ATTRIB );
	glVertexAttribPointer(
		pipeline::VERTEX_ATTRIB,
		ARRAY_SIZE_FROM_ELEMENTS( s_vertex::pos.v ),
		GL_FLOAT,
		GL_FALSE, 
		sizeof( s_vertex ),
		(GLvoid*)offsetof( s_vertex, pos.v )
	);
	
	//send the UVs to opengl
	glEnableVertexAttribArray( pipeline::TEXTURE_ATTRIB );
	glVertexAttribPointer(
		pipeline::TEXTURE_ATTRIB,
		ARRAY_SIZE_FROM_ELEMENTS( s_vertex::uv.v ),
		GL_FLOAT,
		GL_FALSE,
		sizeof( s_vertex ),
		(GLvoid*)offsetof( s_vertex, uv.v )
	);
	
	//send the normals to opengl
	glEnableVertexAttribArray( pipeline::NORMAL_ATTRIB );
	glVertexAttribPointer(
		pipeline::NORMAL_ATTRIB,
		ARRAY_SIZE_FROM_ELEMENTS( s_vertex::norm.v ),
		GL_FLOAT,
		GL_FALSE,
		sizeof( s_vertex ),
		(GLvoid*)offsetof( s_vertex, norm.v )
	);
	
	//send the tangents to opengl
	glEnableVertexAttribArray( pipeline::TANGENT_ATTRIB );
	glVertexAttribPointer(
		pipeline::TANGENT_ATTRIB,
		ARRAY_SIZE_FROM_ELEMENTS( s_vertex::tangent.v ),
		GL_FLOAT,
		GL_FALSE,
		sizeof( s_vertex ),
		(GLvoid*)offsetof( s_vertex, tangent.v )
	);
	printGlError( "Error while sending mesh data to the GPU.");
	
	// create an index buffer to the current set of data
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, buffers[ 1 ] );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( indices[0] ) * numIndices, indices, GL_STATIC_DRAW );
	printGlError( "Error while sending mesh data to the GPU.");
}

//-----------------------------------------------------------------------------
// Scene Drawing
//-----------------------------------------------------------------------------
void c_mesh::draw() const {
	glBindVertexArray( vao );
	
	for ( uint i( 0 ); i < numMeshes; ++i ) {
		
		const uint matIndex = entries[ i ].matIndex;
		
		if ( textures )
            textures[ matIndex ].makeActive();
		
		glDrawElementsBaseVertex(
			GL_TRIANGLES, entries[ i ].numIndices, GL_UNSIGNED_INT,
			( GLvoid* )( sizeof( uint ) * entries[ i ].baseIndex ),
			entries[ i ].baseVertex
		);
	}
	glBindVertexArray( 0 );
}

} // end hge namespace
