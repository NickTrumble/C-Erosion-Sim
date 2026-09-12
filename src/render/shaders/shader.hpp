#pragma once

#include <glad/glad.h>
#include <string>

class Shader{
    public:
        Shader(const std::string& vertexPat, const std::string& fragmentPath);
        ~Shader();

        void use() const;

    private:
        unsigned int program = 0;
};