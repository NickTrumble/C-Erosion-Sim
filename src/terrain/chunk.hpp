#pragma once

#include <utility>
#include "heightmap.hpp"

class Chunk{
    public:
        Chunk(int chunkX, int chunkY, int size, int resolution);

        std::pair<int, int> getChunkCoords() const;
        bool isReady() const;

    private:
        int chunkX;
        int chunkY;
        int size;
        int resolution;
        Heightmap heightmap;
        
        bool ready = false;
};