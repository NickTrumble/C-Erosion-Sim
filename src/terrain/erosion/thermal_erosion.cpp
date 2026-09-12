#include "thermal_erosion.hpp"

#include <algorithm>
#include <vector>

void ThermalErosion::apply(Heightmap& heightmap, int iterations, float talusThreshold, float transferRate){
    if (iterations <= 0 || talusThreshold <= 0.0f)
        return;

    int width = heightmap.getWidth();
    int height = heightmap.getHeight();
    float rate = std::clamp(transferRate, 0.0f, 0.5f);

    std::vector<float> heightChanges(width * height);

    int offsets[4][2] = {
        {-1, 0},
        { 1, 0},
        { 0,-1},
        { 0, 1}
    };

    for (int iter = 0; iter < iterations; iter++)
    {
        std::fill(heightChanges.begin(), heightChanges.end(), 0.0f);

        for (int i = 1; i < width - 1; i++)
        {
            for (int j = 1; j < height - 1; j++)
            {
                float currentHeight = heightmap.getHeightAt(i, j);

                float steepestDrop = talusThreshold;
                int lowestX = i;
                int lowestY = j;

                for (auto& offset : offsets)
                {
                    int neighbourX = i + offset[0];
                    int neighbourY = j + offset[1];

                    float neighbourHeight = heightmap.getHeightAt(neighbourX, neighbourY);

                    float drop = currentHeight - neighbourHeight;
                    if (drop > steepestDrop){
                        steepestDrop = drop;
                        lowestX = neighbourX;
                        lowestY = neighbourY;
                    }
                }
                
                if (lowestX == i && lowestY == j)
                    continue;

                float movedHeight = (steepestDrop - talusThreshold) * rate;

                heightChanges[j * width + i] -= movedHeight;
                heightChanges[lowestY * width + lowestX] += movedHeight;
            }
        }
        

        for (int i = 0; i < width; i++)
        {
            for (int j = 0; j < height; j++)
            {
                heightmap.setHeightAt(i, j, heightmap.getHeightAt(i, j) + heightChanges[j * width + i]);
            }
            
        }
        
    }
    
}