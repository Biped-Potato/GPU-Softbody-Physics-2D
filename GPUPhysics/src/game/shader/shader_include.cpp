#include "shader_include.h"

ShaderInclude::ShaderInclude(const char* path,const char* shaderName)
{
    // 1. retrieve the vertex/fragment source code from filePath
    std::string shaderCode;
    std::ifstream shaderFile;
    // ensure ifstream objects can throw exceptions:
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        shaderFile.open(path);
        std::stringstream shaderStream;
        // read file's buffer contents into streams
        shaderStream << shaderFile.rdbuf();
        // close file handlers
        shaderFile.close();
        // convert stream into string
        shaderCode = shaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char* c_shaderCode = shaderCode.c_str();

    unsigned int shader;

    
    glNamedStringARB(GL_SHADER_INCLUDE_ARB, strlen(shaderName), shaderName, strlen(c_shaderCode), c_shaderCode);
}