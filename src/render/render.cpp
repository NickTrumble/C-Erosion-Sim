#include <GLFW/glfw3.h>
#include "render.hpp"
#include <algorithm>
#include <vector>

void Render::drawHeightmap(const Heightmap& heightmap, int windowWidth, int windowHeight){
    int width = heightmap.getWidth();
    int height = heightmap.getHeight();

    std::vector<unsigned char> pixels(width * height * 3);

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            float val = heightmap.getHeightAt(i, j);
            float normalised = 0.5f * (val + 1.0f);
            normalised = std::clamp(normalised, 0.0f, 1.0f);

            unsigned char grey = static_cast<unsigned char>(normalised * 255.0f);

            int index = (j * width + i) * 3;
            pixels[index++] = grey;
            pixels[index++] = grey;
            pixels[index] = grey; 
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
