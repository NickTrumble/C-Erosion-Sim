#pragma once

#include <glad/glad.h>
#include <string>

class Shader{
    public:
        Shader(const std::string& vertexPat, const std::string& fragmentPath);
        ~Shader();

        void use() const;
        void setInt(const char* name, int value) const;
        void setFloat(const char* name, float value) const;
        void setVec2(const char* name, float x, float y) const;

    private:
        unsigned int program = 0;
};
