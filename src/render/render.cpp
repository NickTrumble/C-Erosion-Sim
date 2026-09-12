#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <algorithm>
#include <string>
#include <vector>
#include "render.hpp"
#include "../app/app_state.hpp"

Render::Render()
    : terrainShader(
        std::string(TERRAIN_SHADER_DIR) + "/terrain.vert",
        std::string(TERRAIN_SHADER_DIR) + "/terrain.frag")
{
    glGenVertexArrays(1, &screenVao);

    glGenTextures(1, &heightTexture);

    glBindTexture(GL_TEXTURE_2D, heightTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

Render::~Render() {
    glDeleteTextures(1, &heightTexture);
    glDeleteVertexArrays(1, &screenVao);
}

void Render::drawHeightmap(const Heightmap& heightmap, int windowWidth, int windowHeight, AppState state){
    glViewport(0, 0, windowWidth, windowHeight);
    terrainShader.use();

    terrainShader.setInt("heightMap", 0);
    terrainShader.setFloat("minHeight", minHeight);
    terrainShader.setFloat("maxHeight", maxHeight);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, heightTexture);

    glBindVertexArray(screenVao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}

void Render::uploadHeightmap(const Heightmap& heightmap) {
    const auto [min, max] = heightmap.findHeightRange();
    minHeight = min;
    maxHeight = max;

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, heightTexture);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_R32F,
        heightmap.getWidth(),
        heightmap.getHeight(),
        0,
        GL_RED,
        GL_FLOAT,
        heightmap.getValues().data()
    );
}