
#include <iostream>
#include "shader.h"

using namespace std;

namespace {
//-----------------------------------------------------------------------------
//	Shader - Error Handling
//-----------------------------------------------------------------------------
void printShaderError(GLuint programId, GLuint shaderId ) {
    printGlError("Error with shader subroutine");
    int infoLogLength = 0;
    GLchar* programInfoLog = 0;
    GLchar* shaderInfoLog = 0;

    if (programId) {
        //get the program info log, followed by the shader information log
        glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &infoLogLength);
        programInfoLog = new GLchar[ infoLogLength + 1 ];
        programInfoLog [infoLogLength ] = 0;

        glGetProgramInfoLog(programId, infoLogLength, nullptr, programInfoLog);
        std::cout << "Program error log:\t";
        std::cout << programInfoLog << "\n";
        delete [] programInfoLog;
    }

    if (shaderId) {
        //Print the shader information log after getting its length
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
        shaderInfoLog = new GLchar[ infoLogLength + 1 ];
        shaderInfoLog[ infoLogLength ] = 0;

        glGetShaderInfoLog(shaderId, infoLogLength, nullptr, shaderInfoLog);
        std::cout << "Shader error log:\n";
        std::cout << "Program: " << programId << "\n";
        std::cout << "Shader: " << shaderId << "\n";
        std::cout << shaderInfoLog;

        delete [] shaderInfoLog;
    }
    std::cout << std::endl;
}

} // end anonymous namespace

///////////////////////////////////////////////////////////////////////////////
//		OpenGL Shader Class
///////////////////////////////////////////////////////////////////////////////
namespace hge {

//-----------------------------------------------------------------------------
//	Shader - Compilation
//-----------------------------------------------------------------------------
bool shader::compile() {
    if (shaderIds.empty()) {
        std::cerr << "WARNING: Attempted to compile a shader program without input" << std::endl;
        return false;
    }
    GLint shaderStatus(0);
    
    glLinkProgram(programId);
    glGetProgramiv(programId, GL_LINK_STATUS, &shaderStatus);
    if (shaderStatus != GL_TRUE) {
        std::cerr << "\nWARNING: A GLSL Shader Error has occurred\n";
        for ( int i = 0; i < shaderIds.size(); ++i )
            printShaderError(programId, shaderIds[ i ]);
        return false;
    }

    printGlError("Error compiling shader");
    return true;
}

//-----------------------------------------------------------------------------
//	Shader - Generic Shader File Loading
//-----------------------------------------------------------------------------
bool shader::load( const char* shaderFilePath, int shaderType ) {
    //check to see if the fragment shader file exists
    if ( !resource::fileExists( shaderFilePath ) ) {
        std::cerr << "WARNING: Attempted to load a nonexistent shader file:\n";
        std::cerr << "\t" << shaderFilePath << std::endl;
        return false;
    }

    int fileLength( 0 );
    char* shaderData( nullptr );
    bool ret = false;

    fileLength = (int)resource::fileSize( shaderFilePath );
    shaderData = new char[ fileLength ];
    resource::readFile( shaderFilePath, shaderData, fileLength );
    
    ret = loadBuffer( shaderData, fileLength, shaderType );
    delete [] shaderData;

    return ret;
}

//-----------------------------------------------------------------------------
//	Shader - Shader From a Buffer
//-----------------------------------------------------------------------------
bool shader::loadBuffer( const char* buffer, int length, int shaderType ) {
    GLint shaderStatus(0);
    GLuint shaderId( 0 );
    
    if ( !programId ) {
        programId = glCreateProgram();
        
        if ( !programId ) {
            std::cerr << "ERROR: Unable to create a shader program." << std::endl;
            return false;
        }
    }
    

    shaderId = glCreateShader( shaderType ); // Fragment shader or Vertex Shader
    glShaderSource( shaderId, 1, (const GLchar**)&buffer, (const GLint*)&length );

    glCompileShader( shaderId );
    glGetShaderiv( shaderId, GL_COMPILE_STATUS, &shaderStatus );

    if ( shaderStatus != GL_TRUE ) {
        printShaderError( 0, shaderId );
        return false;
    }
    
    glAttachShader( programId, shaderId );
    shaderIds.push_back( shaderId );

    printGlError("General shader loading error");
    return true;
}

//-----------------------------------------------------------------------------
//	Shader - Shader Unloading
//-----------------------------------------------------------------------------
void shader::unload() {
    for (unsigned i(0); i < shaderIds.size(); ++i) {
        glDetachShader(programId, shaderIds[ i ]);
        glDeleteShader(shaderIds[ i ]);
    }
    shaderIds.clear();

    if (programId) {
        glDeleteProgram(programId);
        programId = 0;
    }
}

//-----------------------------------------------------------------------------
//	Shader - Variable Acquisition
//-----------------------------------------------------------------------------
GLint shader::getVariableId( const char* v ) {
    return glGetUniformLocation( programId, v );
}

} // End hge namespace
