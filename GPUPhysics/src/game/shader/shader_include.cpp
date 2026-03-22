#include "shader_include.h"


void ShaderInclude::addFile(const char * path, const char * shaderName) {
    std::string shader_code;
    std::ifstream shader_file;
    shader_file.open(path);
    std::stringstream stream;
    stream << shader_file.rdbuf();
    shader_file.close();
    shader_code = stream.str();

    include_files[shaderName] = shader_code;
}

std::string ShaderInclude::get(std::string file) {
    return include_files[file];
}

std::string ShaderInclude::replaceIncludes(const char * shader_code) {
    std::regex includeRegex(R"REGEX(#include\s+"([^"]+)")REGEX");
    std::smatch match;
    std::string code = shader_code;
    int i = 0;
    while (std::regex_search(code, match, includeRegex)) {
        i++;
        std::string includePath = match[1].str();
        std::string include_code = this->get(includePath);
        
        include_code = replaceIncludes(include_code.c_str());

        code.replace(match.position(0), match.length(0), include_code);
    }
    return code;
}