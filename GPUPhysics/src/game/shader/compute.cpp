#include "compute.h"

ComputeShader::ComputeShader(const char* computePath, std::vector<char*> include_vec)
{
    // 1. retrieve the vertex/fragment source code from filePath
    std::string computeCode;
    std::ifstream cShaderFile;
    // ensure ifstream objects can throw exceptions:
    cShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        cShaderFile.open(computePath);
        std::stringstream cShaderStream;
        // read file's buffer contents into streams
        cShaderStream << cShaderFile.rdbuf();
        // close file handlers
        cShaderFile.close();
        // convert stream into string
        computeCode = cShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char* cShaderCode = computeCode.c_str();

    unsigned int computeShader;

    computeShader = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(computeShader, 1, &cShaderCode, NULL);

    // Define the include path
    char** includePaths = &include_vec[0];
    std::vector<GLint> length_vec = {};

    for (int i = 0; i < include_vec.size(); i++) {
        length_vec.push_back((GLint)strlen(include_vec[i]));
    }
    GLint* lengths = &length_vec[0];

    // Compile the shader with includes
    glCompileShaderIncludeARB(computeShader, 1, includePaths, lengths);
    Shader::checkCompileErrors(computeShader, "COMPUTE");

    ID = glCreateProgram();

    glAttachShader(ID, computeShader);
    glLinkProgram(ID);

    Shader::checkCompileErrors(ID, "PROGRAM");

    glDeleteProgram(computeShader);
}

void ComputeShader::use() {
    glUseProgram(ID);
}

void ComputeShader::setBool(const std::string& name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void ComputeShader::setInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void ComputeShader::setFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
void ComputeShader::set3DVector(const std::string& name, float r, float g, float b) const
{
    glUniform3f(glGetUniformLocation(ID, name.c_str()), r, g, b);
}
void ComputeShader::set3DVector(const std::string& name, glm::vec3 v) const {
    glUniform3f(glGetUniformLocation(ID, name.c_str()), v.x, v.y, v.z);
}
void ComputeShader::setMatrix(const std::string& name, glm::mat4 matrix) {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}