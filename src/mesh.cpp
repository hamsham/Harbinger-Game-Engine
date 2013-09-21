/* 
 * File:   mesh.cpp
 * Author: hammy
 * 
 * Created on December 30, 2012, 6:00 PM
 */

#include <iostream>
#include <cstring>
#include <utility>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "mesh.h"
#include "geometry.h"

namespace hge {

/******************************************************************************
 * Mesh -- Construction & Move Semantics
******************************************************************************/
mesh::mesh( mesh&& m ) :
    resource    ( std::move( m ) ),
    drawable    ( std::move( m ) ),
    numMeshes   ( m.numMeshes ),
    numTextures ( m.numTextures ),
    buffers     { m.buffers[ 0 ], m.buffers[ 1 ] },
    entries     ( m.entries ),
    textures    ( m.textures )
{
    m.numMeshes     = 0;
    m.numTextures   = 0;
    m.buffers[0]    = m.buffers[1] = 0;
    m.entries       = nullptr;
    m.textures      = nullptr;
}

mesh& mesh::operator = ( mesh&& m ) {
    unload();
    resource::operator =( std::move( m ) );
    drawable::operator =( std::move( m ) );
    
    numMeshes       = m.numMeshes;
    m.numMeshes     = 0;
    
    numTextures     = m.numTextures;
    m.numTextures   = 0;
    
    buffers[ 0 ]    = m.buffers[ 0 ];
    m.buffers[0]    = 0;
    buffers[ 1 ]    = m.buffers[ 1 ];
    m.buffers[1]    = 0;
    
    delete [] entries;
    entries         = m.entries;
    m.entries       = nullptr;
    
    delete [] textures;
    textures        = m.textures;
    m.textures      = nullptr;
    
    return *this;
}

/******************************************************************************
 * Mesh Unloading
******************************************************************************/
void mesh::unload() {
    delete [] entries;
    entries = nullptr;
    numMeshes = 0;

    delete [] textures;
    textures = nullptr;
    numTextures = 0;
    
	glDeleteVertexArrays( 1, &vao );
	vao = 0;
    
	glDeleteBuffers( 2, buffers );
    buffers[ 0 ] = buffers[ 1 ] = 0;
    
    resetDrawMode();
}

/******************************************************************************
 * Mesh load check
******************************************************************************/
bool mesh::isLoaded() const {
	return ( entries != nullptr && textures != nullptr );
}

/******************************************************************************
 * Mesh loading
******************************************************************************/
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
	pScene = importer.ReadFile(
        fileName,
        aiProcess_Triangulate               |
        aiProcess_GenSmoothNormals          |
        aiProcess_GenUVCoords               |
        aiProcess_CalcTangentSpace          |
        aiProcess_JoinIdenticalVertices     |
        aiProcess_SplitLargeMeshes          |
        aiProcess_OptimizeMeshes            |
        aiProcess_FindInstances             |
        aiProcess_ValidateDataStructure     |
        aiProcess_RemoveRedundantMaterials  |
        aiProcess_ImproveCacheLocality      |
        aiProcess_SortByPType               |
        aiProcess_FindDegenerates           |
        aiProcess_FindInvalidData           |
        aiProcess_RemoveComponent           |
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
    resetDrawMode();
	return true;
}

/******************************************************************************
 * Mesh Data preparation
******************************************************************************/
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
		<< "\t\t" << pScene->mNumMaterials << " textures\n"
		<< "\t\t" << numMeshes << " meshes\n";
	return true;
}

/******************************************************************************
 * Mesh loading
******************************************************************************/
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

            if ( pMesh->HasTextureCoords( 0 ) ) {
                vertArray[ vertIter ].uv[ 0 ] = pMesh->mTextureCoords[ 0 ][ j ].x;
                vertArray[ vertIter ].uv[ 1 ] = pMesh->mTextureCoords[ 0 ][ j ].y;
            }
            
            // Calculate our own tangents if Assimp was unable to
            if ( pMesh->HasTangentsAndBitangents() ) {
                const aiVector3D* pTng      = &pMesh->mTangents[ j ];
                const aiVector3D* pBtng     = &pMesh->mBitangents[ j ];
                vertArray[ vertIter ].tng   = vec3( pTng->x, pTng->y, pTng->z );
                vertArray[ vertIter ].btng  = vec3( pBtng->x, pBtng->y, pBtng->z );
            }
            else {
                if ( vertIter >= 2 )
                calcTangents(
                    vertArray[ vertIter - 2 ],
                    vertArray[ vertIter - 1 ],
                    vertArray[ vertIter - 0 ]
                );
            }
            
            ++vertIter;
        }

        // gather all indices from the meshEntry faces
        for ( unsigned f = 0; f < pMesh->mNumFaces; ++f ) {
            const aiFace* face          = &pMesh->mFaces[ f ];
            indexArray[ indexIter+0 ]   = face->mIndices[ 0 ];
            indexArray[ indexIter+1 ]   = face->mIndices[ 1 ];
            indexArray[ indexIter+2 ]   = face->mIndices[ 2 ];
            indexIter += 3;
        }
        std::cout << "\t\tLoaded sub-mesh " << i << ".\n";
    }

    return true;
}

/******************************************************************************
 * Mesh texture loading
******************************************************************************/
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

/******************************************************************************
 * Mesh loading specific texture types
******************************************************************************/
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

/******************************************************************************
 * Mesh uploading to GPU
******************************************************************************/
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
    
    pipeline::enableBumpVertexAttribs();
	
	// create an index buffer to the current set of data
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, buffers[ 1 ] );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( indices[0] ) * numIndices, indices, GL_STATIC_DRAW );
	printGlError( "Error while sending mesh data to the GPU.");
	
	glBindVertexArray( 0 );
}

/******************************************************************************
 * Mesh drawing
******************************************************************************/
void mesh::draw() const {
	glBindVertexArray( vao );
	
	for ( unsigned i( 0 ); i < numMeshes; ++i ) {
		
		const unsigned matIndex = entries[ i ].matIndex;
		
		if ( textures )
            textures[ matIndex ].activate();
		
		glDrawElementsBaseVertex(
			renderMode, entries[ i ].numIndices, GL_UNSIGNED_INT,
			( GLvoid* )( sizeof( unsigned ) * entries[ i ].baseIndex ),
			entries[ i ].baseVertex
		);
		
		if ( textures )
            textures[ matIndex ].deActivate();
	}
	glBindVertexArray( 0 );
}

} // end hge namespace
