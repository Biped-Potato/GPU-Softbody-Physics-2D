#pragma once

#include <glad/glad.h> // include glad to get all the required OpenGL headers

#include <regex>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>

class ShaderInclude
{
private:
    std::map<std::string, std::string> include_files;
public:

    void addFile(const char * path, const char * shaderName);
    std::string get(std::string file);
    std::string replaceIncludes(const char * shader_code);
};

