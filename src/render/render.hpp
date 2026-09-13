#pragma once

#include <glad/glad.h>

#include "../terrain/heightmap.hpp"
#include "../app/app_state.hpp"
#include "camera.hpp"
#include "shaders/shader.hpp"

class Render{
    public:
        Render();
        ~Render();
        void drawHeightmap(int windowWidth, int windowHeight, AppState state, const Camera& camera);
        void uploadHeightmap(const Heightmap& heightmap);
        
    private:
        unsigned int heightTexture = 0;
        float minHeight = 0.0f;
        float maxHeight = 1.0f;
        Shader terrainShader;
        unsigned int screenVao = 0;
};
