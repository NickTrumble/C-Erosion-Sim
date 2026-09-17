#pragma once

#include "../maths/noise/perlin.hpp"
#include "../terrain/heightmap.hpp"
#include "../terrain/terrain_generator.hpp"

void runHeightmapBenchmark(TerrainGeneratorSettings, float scale, int attempts = 5);
