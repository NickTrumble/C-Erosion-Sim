#include "terrain_generator.hpp"
#include "erosion/thermal_erosion.hpp"

Heightmap TerrainGenerator::generate(const TerrainGeneratorSettings& settings, float scale){
    perlin noise(        
        settings.heightmapSize,
        settings.octaves,
        settings.persistence,
        settings.seed
    );
    
    Heightmap heightmap = Heightmap::generateHeightmap(noise, scale);

    ThermalErosion::apply(heightmap, settings.erosionIterations, settings.talusThreshold, settings.transferRate);
    return heightmap;
}
