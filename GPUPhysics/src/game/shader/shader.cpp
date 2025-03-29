#include "shader.h"

void Shader::checkCompileErrors(unsigned int ID, const std::string& shader_type) {
    char infoLog[512];
    int success;
    std::string error_type = "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n";
    if (shader_type == "FRAGMENT") {
        glGetShaderiv(ID, GL_LINK_STATUS, &success);
        error_type = "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n";
    }
    else if (shader_type == "VERTEX") {
        glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
        error_type = "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n";
    }
    else if (shader_type == "PROGRAM") {
        glGetProgramiv(ID, GL_COMPILE_STATUS, &success);
        error_type = "ERROR::SHADER::PROGRAM::LINKING_FAILED\n";
    }
    else if (shader_type == "COMPUTE") {
        glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
        error_type = "ERROR::SHADER::COMPUTE::COMPILATION_FAILED\n";
    }
    //std::cout << shader_type << "\n";
    if (!success) {
        //std::cout << "not success" << "\n";
        if (shader_type == "PROGRAM") {
            glGetProgramInfoLog(ID, 512, NULL, infoLog);

            std::cout << error_type << infoLog << std::endl;
        }
        else {
            glGetShaderInfoLog(ID, 512, NULL, infoLog);

            std::cout << error_type << infoLog << std::endl;
        }
    }
}
Shader::Shader(const char* vertexPath, const char* fragmentPath,std::vector<char*> include_vec)
{
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    
    unsigned int fragmentShader;
    unsigned int vertexShader;


    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vShaderCode, NULL);

    // Define the include path
    char** includePaths = &include_vec[0];
    std::vector<GLint> length_vec = {};

    for (int i = 0; i < include_vec.size(); i++) {
        length_vec.push_back((GLint)strlen(include_vec[i]));
    }
    GLint* lengths = &length_vec[0];

    // Compile the shader with includes
    glCompileShaderIncludeARB(vertexShader, 1, includePaths, lengths);
    checkCompileErrors(vertexShader, "VERTEX");

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
    glCompileShader(fragmentShader);
    checkCompileErrors(fragmentShader, "FRAGMENT");

    ID = glCreateProgram();

    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);

    checkCompileErrors(ID, "PROGRAM");


    glDeleteProgram(vertexShader);
    glDeleteProgram(fragmentShader);


}

void Shader::use() {
    glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::setInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::set3DVector(const std::string& name, float r, float g, float b) const
{
    glUniform3f(glGetUniformLocation(ID, name.c_str()), r,g,b);
}
void Shader::set3DVector(const std::string& name, glm::vec3 v) const {
    glUniform3f(glGetUniformLocation(ID, name.c_str()), v.x,v.y,v.z);
}

void Shader::setMatrix(const std::string& name, glm::mat4 matrix) {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}