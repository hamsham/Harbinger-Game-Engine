
#include <iostream>
#include "global.h"
#include "pipelineGL.h"
#include "shader.h"
using namespace std;

///////////////////////////////////////////////////////////////////////////////
//		OpenGL Shader Class
///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
//	Shader - Static Variables
//-----------------------------------------------------------------------------
const GLuint c_shader::INVALID_UNIFORM	( -1 );
const GLuint c_shader::VERTEX_ATTRIB		( 0 );
const GLuint c_shader::TEXTURE_ATTRIB		( 1 );
const GLuint c_shader::NORMAL_ATTRIB		( 2 );
		
//-----------------------------------------------------------------------------
//	Shader - Shader Structure
//-----------------------------------------------------------------------------
c_shader::c_shader() :
	progID( 0 )
{}

c_shader::~c_shader() {
	unload();
}

//-----------------------------------------------------------------------------
//	Shader - Error Handling
//-----------------------------------------------------------------------------
void c_shader::printError( GLuint shdrID ) const {
	printGLError( "Error with shader subroutine" );
	
	int infoLogLength = 0;
	GLchar* programInfoLog = 0;
	GLchar* shaderInfoLog = 0;
	
	if ( progID ) {
		//get the program info log, followed by the shader information log
		glGetProgramiv( progID, GL_INFO_LOG_LENGTH, &infoLogLength );
		programInfoLog = new GLchar[ infoLogLength+1 ];
		programInfoLog [infoLogLength ] = 0;

		glGetProgramInfoLog( progID, infoLogLength, HGE_NULL, programInfoLog );
		std::cout << "Program error log:\t";
		std::cout << programInfoLog << "\n";
		delete [] programInfoLog;
	}
	
	if ( shdrID ) {
		//Print the shader information log after getting its length
		glGetShaderiv( shdrID, GL_INFO_LOG_LENGTH, &infoLogLength );
		shaderInfoLog = new GLchar[ infoLogLength+1 ];
		shaderInfoLog[ infoLogLength ] = 0;

		glGetShaderInfoLog( shdrID, infoLogLength, HGE_NULL, shaderInfoLog );
		std::cout << "Shader error log:\n";
		std::cout << "Program: " << progID << "\n";
		std::cout << "Shader: " << shdrID << "\n";
		std::cout << shaderInfoLog;

		delete [] shaderInfoLog;
	}
	std::cout << std::endl;
}

//-----------------------------------------------------------------------------
//	Shader - Compilation
//-----------------------------------------------------------------------------
bool c_shader::compile() {
	if ( shaderID.empty() ) {
		std::cerr << "WARNING: Attempted to compile a shader program without input" << std::endl;
		return false;
	}
	
	if ( progID ) {
		unload();
	}
	progID = glCreateProgram();
	GLint shaderStatus( 0 );
	
	// Link the shader files
	for ( uint i( 0 ); i < shaderID.size(); ++i ) {
		glAttachShader( progID, shaderID[ i ] );
		glLinkProgram( progID );
		glGetProgramiv( progID, GL_LINK_STATUS, &shaderStatus);
		if (shaderStatus != GL_TRUE) {
			std::cerr << "\nWARNING: A GLSL Shader Error has occurred\n";
			printError( shaderID[ i ] );
			return false;
		}
	}
	
	printGLError( "Error compiling shader" );
	return true;
}

//-----------------------------------------------------------------------------
//	Shader - Generic Shader File Loading
//-----------------------------------------------------------------------------
bool c_shader::load( cstr shaderFilePath, int shaderType ) {
	//check to see if the fragment shader file exists
	if ( !c_resource::fileExists( shaderFilePath ) ) {
		std::cerr << "WARNING: Attempted to load a nonexistent shader file:\n";
		std::cerr << "\t" << shaderFilePath << std::endl;
		return false;
	}
	
	GLint fileLength( 0 );
	GLchar* shaderData( HGE_NULL );
	GLint shaderStatus( 0 );
	GLuint* pShader( HGE_NULL );
	
	shaderID.push_back( 0 );
	pShader = &shaderID.back();
	
	fileLength = c_resource::fileSize( shaderFilePath );	//get the length of the file in order to allocate room in "fragFile"
	shaderData = new GLchar[ fileLength ];				//load the fragment shader file into memory
	c_resource::readFile( shaderFilePath, (char*)shaderData, fileLength );
	
	*pShader = glCreateShader( shaderType );			// Fragment shader or Vertex Shader
	glShaderSource( *pShader, 1, (const GLchar**)&shaderData, &fileLength );
	delete [] shaderData;
	
	glCompileShader( *pShader );
	glGetShaderiv( *pShader, GL_COMPILE_STATUS, &shaderStatus );
	
	if ( shaderStatus != GL_TRUE ) {
		printGLError( shaderFilePath );
		printError( *pShader );
		shaderID.pop_back();
		return false;
	}
	
	printGLError( "General shader loading error" );
	return true;
}

//-----------------------------------------------------------------------------
//	Shader - Shader Unoading
//-----------------------------------------------------------------------------
void c_shader::unload() {
	for( uint i( 0 ); i < shaderID.size(); ++i ) {
		glDetachShader( progID, shaderID[ i ] );
		glDeleteShader( shaderID[ i ] );
	}
	shaderID.clear();
	
	if (progID) {
		glDeleteProgram( progID );
		progID = 0;
	}
}

///////////////////////////////////////////////////////////////////////////////
//		Stock (prebuilt) Shaders
///////////////////////////////////////////////////////////////////////////////
namespace {
	c_shader texture2d_shdr;
	c_shader ambient_shdr;
	c_shader diffuse_shdr;
	c_shader specular_shdr;
} // end anonymous namespace

//-----------------------------------------------------------------------------
//	Stock Shaders - Initialization
//-----------------------------------------------------------------------------
bool n_stockShaders::init() {
	//Basic 2D Textures
	if (
		!texture2d_shdr.load( "shaders/texture_2d_plain.vs", GL_VERTEX_SHADER )
	||	!texture2d_shdr.load( "shaders/texture_2d_plain.fs", GL_FRAGMENT_SHADER )
	||	!texture2d_shdr.compile()
	) {
		std::cerr << "Error loading 2D texture shader. Aborting" << std::endl;
		return false;
	}
	
	//Basic Ambient Lighting
	if (
		!ambient_shdr.load( "shaders/ambient.vs", GL_VERTEX_SHADER )
	||	!ambient_shdr.load( "shaders/ambient.fs", GL_FRAGMENT_SHADER )
	||	!ambient_shdr.compile()
	) {
		std::cerr << "Error loading ambient lighting shader. Aborting" << std::endl;
		return false;
	}
	
	//Basic Diffuse Lighting
	if (
		!diffuse_shdr.load( "shaders/diffuse.vs", GL_VERTEX_SHADER )
	||	!diffuse_shdr.load( "shaders/diffuse.fs", GL_FRAGMENT_SHADER )
	||	!diffuse_shdr.compile()
	) {
		std::cerr << "Error loading diffuse lighting shader. Aborting" << std::endl;
		return false;
	}
	
	//Basic Specular Lighting
	if (
		!specular_shdr.load( "shaders/specular.vs", GL_VERTEX_SHADER )
	||	!specular_shdr.load( "shaders/specular.fs", GL_FRAGMENT_SHADER )
	||	!specular_shdr.compile()
	) {
		std::cerr << "Error loading specular lighting shader. Aborting" << std::endl;
		return false;
	}
	
	// all shaders initialized
	return true;
}

//-----------------------------------------------------------------------------
//	Stock Shaders - Resource Acquisition
//-----------------------------------------------------------------------------
GLuint n_stockShaders::getTexture2dShader		() { return texture2d_shdr.getProgramID(); }
GLuint n_stockShaders::getAmbientLightShader	() { return ambient_shdr.getProgramID(); }
GLuint n_stockShaders::getDiffuseLightShader	() { return diffuse_shdr.getProgramID(); }
GLuint n_stockShaders::getSpecularLightShader	() { return specular_shdr.getProgramID(); }
