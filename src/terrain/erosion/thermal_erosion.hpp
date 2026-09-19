#pragma once

#include <vector>

#include "../heightmap.hpp"

class ThermalErosion{
    public:
        static void apply(Heightmap& heightmap,int iterations,float talusThreshold,float transferRate);

    private:
        static void calculateOutflow(
            std::vector<float>& values,
            std::vector<float>& outflow,
            std::vector<int>& destination,
            int width, int firstRow, int lastRow,
            float talusThreshold, float transferRate,
            int height
        );

        static void gatherInflow(
            std::vector<float>& values,
            std::vector<float>& outflow,
            std::vector<int>& destination,
            std::vector<float>& nextValues,
            int width, int firstRow, int lastRow,
            int height
        );

};
