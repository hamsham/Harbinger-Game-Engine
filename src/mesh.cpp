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
#include "mesh.h"

namespace hge {

///////////////////////////////////////////////////////////////////////////////
//	Mesh Structure
///////////////////////////////////////////////////////////////////////////////
mesh::meshEntry::meshEntry() :
	matIndex	( 0 ),
	baseVertex	( 0 ),
	baseIndex	( 0 ),
	numIndices	( 0 )
{}

mesh::meshEntry::meshEntry( const mesh::meshEntry& meCopy) :
	matIndex	( meCopy.matIndex ),
	baseVertex	( meCopy.baseVertex ),
	baseIndex	( meCopy.baseIndex ),
	numIndices	( meCopy.numIndices )
{}

mesh::meshEntry& mesh::meshEntry::operator = ( const mesh::meshEntry& meCopy ) {
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
// Mesh - Move Operators
//-----------------------------------------------------------------------------
mesh::mesh( mesh&& m ) :
    resource( m ),
    numMeshes( m.numMeshes ),
    numTextures( m.numTextures ),
    vao( m.vao ),
    buffers{ m.buffers[ 0 ], m.buffers[ 1 ] },
    entries( m.entries ),
    textures( m.textures )
{
    m.entries = nullptr;
    m.textures = nullptr;
}

mesh& mesh::operator = ( mesh&& m ) {
    numMeshes       = m.numMeshes;
    numTextures     = m.numTextures;
    vao             = m.vao;
    buffers[ 0 ]    = m.buffers[ 0 ];
    buffers[ 1 ]    = m.buffers[ 1 ];
    entries         = m.entries;
    textures        = m.textures;
    
    m.entries = nullptr;
    m.textures = nullptr;
}

//-----------------------------------------------------------------------------
// Mesh - Unloading
//-----------------------------------------------------------------------------
void mesh::unload() {
	if ( entries ) {
		delete [] entries;
		entries = nullptr;
		numMeshes = 0;
	}
	if ( textures ) {
		delete [] textures;
		textures = nullptr;
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
bool mesh::isLoaded() const {
	return ( entries != nullptr && textures != nullptr );
}

//-----------------------------------------------------------------------------
// Mesh - Loading
//-----------------------------------------------------------------------------
bool mesh::load( const char* fileName, int flags ) {
	std::cout << "Loading 3d mesh data:" << "\n\tFile:\t" << fileName << "\n";
	
	unload();
	Assimp::Importer importer;
	const aiScene* pScene   = nullptr;
	bumpVertex* vertArray     = nullptr;
	unsigned* indexArray        = nullptr;
	unsigned numVerts           = 0;
	unsigned numIndices         = 0;
	
	std::cout << "\tReading data from the file...\n";
	//pScene = importer.ReadFile( fileName, aiProcessPreset_TargetRealtime_MaxQuality );
	pScene = importer.ReadFile( fileName,   aiProcess_Triangulate           |
                                            aiProcess_GenSmoothNormals      |
                                            aiProcess_GenUVCoords           |
                                            aiProcess_CalcTangentSpace      |
                                            aiProcess_JoinIdenticalVertices |
                                            aiProcess_SplitLargeMeshes      |
                                            aiProcess_OptimizeMeshes        |
                                            aiProcess_FindInstances         |
                                            aiProcess_ValidateDataStructure |
                                            aiProcess_RemoveRedundantMaterials |
                                            aiProcess_ImproveCacheLocality  |
                                            aiProcess_SortByPType           |
                                            aiProcess_FindDegenerates       |
                                            aiProcess_FindInvalidData       |
                                            aiProcess_RemoveComponent       |
                                            0
    );
	if ( pScene == nullptr ) {
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
	vertArray   = new bumpVertex[ numVerts ];
	indexArray  = new unsigned[ numIndices ];
	entries     = new meshEntry[ numMeshes ];
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
	textures = new bitmap [ numTextures ];
	if ( !loadTextures( pScene, fileName ) ) {
		std::cout << "\t\tWarning: Unable to load all associated materials.\n";
		delete [] textures;
		textures = nullptr;
		numTextures = 0;
	}
	std::cout << "\tDone.\n";
	
	std::cout << "\tSending mesh data to the GPU...\n";
	loadVao( vertArray, numVerts, indexArray, numIndices );
	std::cout << "\tDone.\n";
	delete [] vertArray;
	delete [] indexArray;
	
	std::cout << "Successfully loaded " << fileName << std::endl;
	return true;
}

//-----------------------------------------------------------------------------
// Mesh - Data preparation process
//-----------------------------------------------------------------------------
bool mesh::prepMeshes( const aiScene* pScene, unsigned& numVerts, unsigned& numIndices ) {
	numVerts    = 0;
	numIndices  = 0;
	numMeshes   = pScene->mNumMeshes;
	
	for ( unsigned i = 0; i < numMeshes; ++i ) {
		const aiMesh* pMesh = pScene->mMeshes[ i ];
		
		for ( unsigned f = 0; f < pMesh->mNumFaces; ++f ) {
			if ( pMesh->mFaces[ f ].mNumIndices != 3 ) {
				std::cout << "\tError: Mesh contains non-triangulated faces\n";
				numVerts = 0;
				numIndices = 0;
				return false;
			}
			numIndices += pMesh->mFaces[ f ].mNumIndices;
		}
		
		for ( unsigned j = 0; j < pMesh->mNumVertices; ++j ) {
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
bool mesh::loadMeshes( const aiScene* pScene, bumpVertex* vertArray, unsigned* indexArray ) {
	const aiMesh* pMesh( nullptr );
    unsigned vertIter   = 0;
    unsigned indexIter  = 0;
    unsigned sumVerts   = 0;
    unsigned sumIndices = 0;

    for ( unsigned i = 0; i < numMeshes; ++i ) {
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
        for ( unsigned j = 0; j < pMesh->mNumVertices; ++j ) {
            const aiVector3D* pPos          = &pMesh->mVertices[ j ];
            const aiVector3D* pNorm         = &pMesh->mNormals[ j ];
            vertArray[ vertIter ].pos       = vec3( pPos->x, pPos->y, pPos->z );
            vertArray[ vertIter ].norm      = vec3( pNorm->x, pNorm->y, pNorm->z );
            
            if ( pMesh->HasTangentsAndBitangents() ) {
                const aiVector3D* pTangent = &pMesh->mTangents[ j ];
                vertArray[ vertIter ].tangent = vec3( pTangent->x, pTangent->y, pTangent->z );
            }

            if ( pMesh->HasTextureCoords( 0 ) ) {
                vertArray[ vertIter ].uv[ 0 ] = pMesh->mTextureCoords[ 0 ][ j ].x;
                vertArray[ vertIter ].uv[ 1 ] = pMesh->mTextureCoords[ 0 ][ j ].y;
            }
            ++vertIter;
        }

        // gather all indices from the meshEntry faces
        for ( unsigned f = 0; f < pMesh->mNumFaces; ++f ) {
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
bool mesh::loadTextures( const aiScene* pScene, const char* fileName ) {
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
    for ( unsigned i = 0; i < numTextures; ++i ) {

        const aiMaterial* pMaterial = pScene->mMaterials[ i ];

        if ( pMaterial->GetTextureCount( aiTextureType_DIFFUSE ) > 0 )
            loadTexType( i, pMaterial, aiTextureType_DIFFUSE, currDir );

    }

    return true;
}

//-----------------------------------------------------------------------------
// Mesh - Loading Specific Texture Types
//-----------------------------------------------------------------------------
void mesh::loadTexType(
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
void mesh::loadVao(
	bumpVertex* vertices, unsigned numVertices,
	unsigned* indices, unsigned numIndices
) {
	// create a buffer for the vertex positions & indices
	glGenVertexArrays( 1, &vao );
	glBindVertexArray( vao );
	glGenBuffers( 2, buffers );
	
	glBindBuffer( GL_ARRAY_BUFFER, buffers[ 0 ] );
	glBufferData( GL_ARRAY_BUFFER, sizeof( bumpVertex ) * numVertices, vertices, GL_STATIC_DRAW );
	printGlError( "Error while sending mesh data to the GPU.");
	
	//send the vertices to opengl
	glEnableVertexAttribArray( pipeline::VERTEX_ATTRIB );
	glVertexAttribPointer(
		pipeline::VERTEX_ATTRIB,
		ARRAY_COUNT_FROM_SIZE( bumpVertex::pos.v ),
		GL_FLOAT,
		GL_FALSE, 
		sizeof( bumpVertex ),
		(GLvoid*)offsetof( bumpVertex, pos.v )
	);
	
	//send the UVs to opengl
	glEnableVertexAttribArray( pipeline::TEXTURE_ATTRIB );
	glVertexAttribPointer(
		pipeline::TEXTURE_ATTRIB,
		ARRAY_COUNT_FROM_SIZE( bumpVertex::uv.v ),
		GL_FLOAT,
		GL_FALSE,
		sizeof( bumpVertex ),
		(GLvoid*)offsetof( bumpVertex, uv.v )
	);
	
	//send the normals to opengl
	glEnableVertexAttribArray( pipeline::NORMAL_ATTRIB );
	glVertexAttribPointer(
		pipeline::NORMAL_ATTRIB,
		ARRAY_COUNT_FROM_SIZE( bumpVertex::norm.v ),
		GL_FLOAT,
		GL_FALSE,
		sizeof( bumpVertex ),
		(GLvoid*)offsetof( bumpVertex, norm.v )
	);
	
	//send the tangents to opengl
	glEnableVertexAttribArray( pipeline::TANGENT_ATTRIB );
	glVertexAttribPointer(
		pipeline::TANGENT_ATTRIB,
		ARRAY_COUNT_FROM_SIZE( bumpVertex::tangent.v ),
		GL_FLOAT,
		GL_FALSE,
		sizeof( bumpVertex ),
		(GLvoid*)offsetof( bumpVertex, tangent.v )
	);
	printGlError( "Error while sending mesh data to the GPU.");
	
	// create an index buffer to the current set of data
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, buffers[ 1 ] );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( indices[0] ) * numIndices, indices, GL_STATIC_DRAW );
	printGlError( "Error while sending mesh data to the GPU.");
	
	glBindVertexArray( 0 );
}

//-----------------------------------------------------------------------------
// Scene Drawing
//-----------------------------------------------------------------------------
void mesh::draw() const {
	glBindVertexArray( vao );
	
	for ( unsigned i( 0 ); i < numMeshes; ++i ) {
		
		const unsigned matIndex = entries[ i ].matIndex;
		
		if ( textures )
            textures[ matIndex ].activate();
		
		glDrawElementsBaseVertex(
			GL_TRIANGLES, entries[ i ].numIndices, GL_UNSIGNED_INT,
			( GLvoid* )( sizeof( unsigned ) * entries[ i ].baseIndex ),
			entries[ i ].baseVertex
		);
		
		if ( textures )
            textures[ matIndex ].deActivate();
	}
	glBindVertexArray( 0 );
}

} // end hge namespace
