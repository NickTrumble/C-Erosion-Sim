#pragma once

#include <vector>

#include "../heightmap.hpp"

class ThermalErosion{
    public:
        static void apply(Heightmap& heightmap,int iterations,float talusThreshold,float transferRate);

    private:
        static void calculateChangesForRows(
            std::vector<float>& values,
            std::vector<float>& localChanges,
            int width,
            int firstRow,
            int lastRow,
            float talusThreshold,
            float transferRate
        );

        static void combineWorkerChanges(
            std::vector<std::vector<float>>& workerChanges,
            std::vector<float>& heightChanges
        );

        static void applyHeightChanges(
            std::vector<float>& values,
            std::vector<float>& heightChanges
        );
};
