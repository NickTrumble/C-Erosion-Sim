#include <GLFW/glfw3.h>
#include <algorithm>
#include <vector>
#include "render.hpp"
#include "../app/app_state.hpp"


void Render::drawHeightmap(const Heightmap& heightmap, int windowWidth, int windowHeight, AppState state){
    int width = heightmap.getWidth();
    int height = heightmap.getHeight();

    std::vector<unsigned char> pixels(width * height * 3);
    std::pair<float, float> minMax = heightmap.findHeightRange();

    for (int i = 0; i < width; i++) 
    {
        for (int j = 0; j < height; j++)
        {
            float val = heightmap.getHeightAt(i, j);

            int index = (j * width + i) * 3;
            std::array<unsigned char, 3> colour = ColourMap::colourToHeight(val, state.type, minMax.first, minMax.second);
            pixels[index++] = colour[0];
            pixels[index++] = colour[1];
            pixels[index] = colour[2]; 
        }        
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    glRasterPos2f(-1.0f, -1.0f);

    glPixelZoom(
        static_cast<float>(windowWidth) / width,
        static_cast<float>(windowHeight) / height    
    );

    glDrawPixels(
        width,
        height,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        pixels.data()
    );

    glPixelZoom(1.0f, 1.0f);
}
