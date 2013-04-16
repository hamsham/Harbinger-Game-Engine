
#include <iostream>
#include "types.h"
#include "pipeline.h"
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

        glGetProgramInfoLog(programId, infoLogLength, HGE_NULL, programInfoLog);
        std::cout << "Program error log:\t";
        std::cout << programInfoLog << "\n";
        delete [] programInfoLog;
    }

    if (shaderId) {
        //Print the shader information log after getting its length
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
        shaderInfoLog = new GLchar[ infoLogLength + 1 ];
        shaderInfoLog[ infoLogLength ] = 0;

        glGetShaderInfoLog(shaderId, infoLogLength, HGE_NULL, shaderInfoLog);
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
//	Shader - Shader Structure
//-----------------------------------------------------------------------------
c_shader::c_shader() :
    progID(0)
{}

c_shader::~c_shader() {
    unload();
}

//-----------------------------------------------------------------------------
//	Shader - Compilation
//-----------------------------------------------------------------------------
bool c_shader::compile() {
    if (shaderID.empty()) {
        std::cerr << "WARNING: Attempted to compile a shader program without input" << std::endl;
        return false;
    }

    if (progID) {
        glDeleteProgram(progID);
        progID = 0;
    }
    progID = glCreateProgram();
    GLint shaderStatus(0);

    // Link the shader files
    for (uint i(0); i < shaderID.size(); ++i) {
        glAttachShader(progID, shaderID[ i ]);
        glLinkProgram(progID);
        glGetProgramiv(progID, GL_LINK_STATUS, &shaderStatus);
        if (shaderStatus != GL_TRUE) {
            std::cerr << "\nWARNING: A GLSL Shader Error has occurred\n";
            printShaderError(progID, shaderID[ i ]);
            return false;
        }
    }

    printGlError("Error compiling shader");
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

    int fileLength( 0 );
    char* shaderData( HGE_NULL );
    bool ret = false;

    fileLength = (int)c_resource::fileSize( shaderFilePath );
    shaderData = new char[ fileLength ];
    c_resource::readFile( shaderFilePath, shaderData, fileLength );
    
    ret = loadBuffer( shaderData, fileLength, shaderType );
    delete [] shaderData;

    return ret;
}

//-----------------------------------------------------------------------------
//	Shader - Shader From a Buffer
//-----------------------------------------------------------------------------
bool c_shader::loadBuffer( cstr buffer, int length, int shaderType ) {
    GLint shaderStatus(0);
    GLuint shader( 0 );

    shader = glCreateShader( shaderType ); // Fragment shader or Vertex Shader
    glShaderSource( shader, 1, (const GLchar**)&buffer, (const GLint*)&length );

    glCompileShader( shader );
    glGetShaderiv( shader, GL_COMPILE_STATUS, &shaderStatus );

    if ( shaderStatus != GL_TRUE ) {
        printShaderError( 0, shader );
        return false;
    }
    
    shaderID.push_back( shader );

    printGlError("General shader loading error");
    return true;
}

//-----------------------------------------------------------------------------
//	Shader - Shader Unloading
//-----------------------------------------------------------------------------
void c_shader::unload() {
    for (uint i(0); i < shaderID.size(); ++i) {
        glDetachShader(progID, shaderID[ i ]);
        glDeleteShader(shaderID[ i ]);
    }
    shaderID.clear();

    if (progID) {
        glDeleteProgram(progID);
        progID = 0;
    }
}

} // End hge namespace
