#pragma once
#include <array>
#include "../terrain/heightmap.hpp"

class Render{
    public:
        void drawHeightmap(const Heightmap& heightmap, int windowWidth, int windowHeight);
        
};
