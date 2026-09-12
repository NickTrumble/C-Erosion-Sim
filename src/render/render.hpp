#pragma once
#include <array>
#include "../terrain/heightmap.hpp"
#include "../app/app_state.hpp"

class Render{
    public:
        void drawHeightmap(const Heightmap& heightmap, int windowWidth, int windowHeight, AppState state);
        
};
