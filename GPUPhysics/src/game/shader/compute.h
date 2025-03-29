#pragma once
#include <glad/glad.h> // include glad to get all the required OpenGL headers

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"

class ComputeShader
{
public:
    // the program ID
    unsigned int ID;

    // constructor reads and builds the shader
    ComputeShader(const char* computePath, std::vector<char*> include_vec);
    // use/activate the shader
    void use();


    // utility uniform functions
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void set3DVector(const std::string& name, float r, float g, float b) const;
    void set3DVector(const std::string& name, glm::vec3 v) const;
    void setMatrix(const std::string& name, glm::mat4 matrix);
};
