#pragma once

#include "heightmap.hpp"

struct TerrainGeneratorSettings{
    int heightmapSize = 512;
    int octaves = 6;
    float persistence = 0.5f;
    float scale = 0.02f;
    int seed = 0;
    float transferRate = 0.15f;
    float talusThreshold = 0.03f;
    int erosionIterations = 50;
    int erosionIterationsPerFrame = 1;
};

class TerrainGenerator{
    public:
        static Heightmap generate(const TerrainGeneratorSettings& settings);
        static Heightmap generateBase(const TerrainGeneratorSettings& settings);
};
