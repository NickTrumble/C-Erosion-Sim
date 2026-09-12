#pragma once

#include "../heightmap.hpp"

class ThermalErosion{
    public:
        static void apply(Heightmap& heightmap,int iterations,float talusThreshold,float transferRate);
};