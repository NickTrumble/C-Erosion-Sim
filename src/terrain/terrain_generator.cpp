#include "terrain_generator.hpp"
#include "erosion/thermal_erosion.hpp"

Heightmap TerrainGenerator::generate(const TerrainGeneratorSettings& settings){
    Heightmap heightmap = generateBase(settings);

    ThermalErosion::apply(heightmap, settings.erosionIterations, settings.talusThreshold, settings.transferRate);
    return heightmap;
}

Heightmap TerrainGenerator::generateBase(const TerrainGeneratorSettings& settings){
    perlin noise(        
        settings.heightmapSize,
        settings.octaves,
        settings.persistence,
        settings.seed
    );
    
    Heightmap heightmap = Heightmap::generateHeightmap(noise, settings.scale);
    return heightmap;
}
