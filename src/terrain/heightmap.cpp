#include "heightmap.hpp"

Heightmap::Heightmap(int width, int height):
    width(width),
    height(height),
    values(width * height, 0.0f)
    {}

int Heightmap::getWidth() const {
    return width;
}

int Heightmap::getHeight() const {
    return height;
}

float Heightmap::getHeightAt(int x, int y) const{
    return values[index(x,y)];
}

void Heightmap::setHeightAt(int x, int y, float val){
    values[index(x, y)] = val;
}

int Heightmap::index(int x, int y) const{
    return y * width + x;
}

Heightmap Heightmap::generateHeightmap(perlin& noise, float scale){
    int size = noise.getSize();
    Heightmap heightmap(size, size);

    for (int i = 0; i < size; i++){
        for (int j = 0; j < size; j++){
            float val = noise.noiseMethod(i * scale, j * scale);
            heightmap.setHeightAt(i, j, val);
        }
    }
    return heightmap;
}

std::pair<float, float> Heightmap::findHeightRange() const{
    float min = 10;
    float max = -10;
    float val = 0;
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            val = getHeightAt(i, j);
            if (val > max){
                max = val;
            } else if (val < min){
                min = val;
            }
        }
    }
    return std::pair<float, float>(min, max);
}