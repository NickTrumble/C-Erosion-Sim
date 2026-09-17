#include "thermal_erosion.hpp"

#include <algorithm>
#include <vector>
#include <thread>

void ThermalErosion::apply(Heightmap& heightmap, int iterations, float talusThreshold, float transferRate){
    if (iterations <= 0 || talusThreshold <= 0.0f)
        return;

    auto& values = heightmap.getValues();
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

    int size = heightmap.getWidth();

    unsigned availableThreads = std::thread::hardware_concurrency();
    int workerCount = std::min(
        size,
        static_cast<int>(availableThreads == 0 ? 1 : availableThreads)
    );

    int rowsPerWorker = (size + workerCount - 1) / workerCount;
    std::vector<std::thread> workers;
    workers.reserve(workerCount);

    std::vector<std::vector<float>> workerChanges(workerCount, std::vector<float>(width * height, 0.0f));

    for (int iter = 0; iter < iterations; iter++)
    {
        std::fill(heightChanges.begin(), heightChanges.end(), 0.0f);
        for (auto& localChanges : workerChanges) {
            std::fill(localChanges.begin(), localChanges.end(), 0.0f);
        }


        for (int worker = 0; worker < workerCount; worker++){
            int firstRow = std::max(1, worker * rowsPerWorker);
            int lastRow = std::min(height - 1, (worker + 1) * rowsPerWorker);

            auto& localChanges = workerChanges[worker];

            workers.emplace_back([
                firstRow,
                lastRow,
                &heightmap,
                size,
                talusThreshold,
                offsets,
                rate,
                &localChanges,
                width,
                &values
            ]{
                for (int j = firstRow; j < lastRow; j++)
                {
                    for (int i = 1; i < size - 1; i++)
                    {
                        float currentHeight = values[j * width + i];

                        float steepestDrop = talusThreshold;
                        int lowestX = i;
                        int lowestY = j;

                        for (auto& offset : offsets)
                        {
                            int neighbourX = i + offset[0];
                            int neighbourY = j + offset[1];

                            float neighbourHeight = values[neighbourY * width + neighbourX];

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

                        localChanges[j * size + i] -= movedHeight;
                        localChanges[lowestY * size + lowestX] += movedHeight;
                    }   
                }
            });

        }

        for (std::thread& worker : workers) {
            worker.join();
        }
        workers.clear();        

        for (auto& localChanges : workerChanges){
            for (int i = 0; i < width * height; i++)
            {
                heightChanges[i] += localChanges[i];
            }
            
        }

        for (int i = 0; i < width; i++)
        {
            for (int j = 0; j < height; j++)
            {
                values[j * width + i] += heightChanges[j * width + i];
            }
            
        }
        
    }
    
}
