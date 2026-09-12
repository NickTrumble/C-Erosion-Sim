#include "shader.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>


namespace 
{
    std::string readFile(const std::string& path){
        std::ifstream file(path); //reads a shader file into one string
        if (!file) {
            throw std::runtime_error("Couldn't open shader: " + path);
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    unsigned int compile(unsigned int type, const std::string& source){
        unsigned int shader = glCreateShader(type);
        const char* text = source.c_str();

        glShaderSource(shader, 1, &text, nullptr);
        glCompileShader(shader);

        int succeeded = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &succeeded);

        if (!succeeded){
            glDeleteShader(shader);
            throw std::runtime_error("shaders not compiled");
        }

        return shader;
    }

} 


Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath){
    const unsigned int vertex = compile(GL_VERTEX_SHADER, readFile(vertexPath));
    const unsigned int fragment = compile(GL_FRAGMENT_SHADER, readFile(fragmentPath));

    program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);

    glLinkProgram(program);

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    int succeeded = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &succeeded);
    if (!succeeded){
        throw std::runtime_error("Shader linking failed");
    }
}

Shader::~Shader() {
    glDeleteProgram(program);
}

void Shader::use() const {
    glUseProgram(program);
}

//sends int and float to shader
void Shader::setInt(const char* name, int value) const {
    glUniform1i(glGetUniformLocation(program, name), value);
}

void Shader::setFloat(const char* name, float value) const {
    glUniform1f(glGetUniformLocation(program, name), value);
}