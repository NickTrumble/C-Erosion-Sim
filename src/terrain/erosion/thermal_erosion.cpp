#include "thermal_erosion.hpp"

#include <algorithm>
#include <array>
#include <thread>
#include <vector>
#include <barrier>

namespace {
std::array<std::array<int, 2>, 4> offsets{{
    {{-1, 0}},
    {{ 1, 0}},
    {{ 0,-1}},
    {{ 0, 1}}
    }};
}

void ThermalErosion::calculateOutflow(
    std::vector<float>& values,
    std::vector<float>& outflow,
    std::vector<int>& destination,
    int width, int firstRow, int lastRow,
    float talusThreshold, float transferRate,
    int height
){
    int first = std::max(1, firstRow);
    int last = std::min(height - 1, lastRow);

    for (int j = first; j < last; j++)
    {
        for (int i = 1; i < width - 1; i++)
        {
            float steepestDrop = talusThreshold;
            int lowestX = i;
            int lowestY = j;
            float currentHeight = values[j * width + i];

            outflow[j * width + i] = 0.0f;
            destination[j * width + i] = j * width + i;

            for (auto& offset : offsets) {
                int neighbourX = i + offset[0];
                int neighbourY = j + offset[1];
                
                float drop = currentHeight - values[neighbourY * width + neighbourX];
                if (drop > steepestDrop){
                    lowestX = neighbourX;
                    lowestY = neighbourY;
                    steepestDrop = drop;
                }
            }

            if (lowestX == i && lowestY == j)
                continue;

            float movedHeight = (steepestDrop - talusThreshold) * transferRate;
            
            outflow[j * width + i] = movedHeight;
            destination[j * width + i] = lowestY * width + lowestX;
        }
        
    }
}   

void ThermalErosion::gatherInflow(
    std::vector<float>& values,
    std::vector<float>& outflow,
    std::vector<int>& destination,
    std::vector<float>& nextValues,
    int width, int firstRow, int lastRow,
    int height
){
    for (int j = firstRow; j < lastRow; j++)
    {
        for (int i = 0; i < width; i++)
        {
            int index = j * width + i;


            float inflow = 0;
            for (auto& offset : offsets) {
                int neighbourX = i + offset[0];
                int neighbourY = j + offset[1];

                if (neighbourX < 0 || neighbourX >= width ||
                    neighbourY < 0 || neighbourY >= height) {
                    continue;
                }

                if (destination[neighbourY * width + neighbourX] != index)
                    continue;
                
                inflow += outflow[neighbourY * width + neighbourX];
            }
            nextValues[index] = values[index] - outflow[index] + inflow;
        }
        
    }
}

void ThermalErosion::apply(Heightmap& heightmap, int iterations, float talusThreshold, float transferRate){
    if (iterations <= 0 || talusThreshold <= 0.0f)
        return;

    auto& values = heightmap.getValues();
    int width = heightmap.getWidth();
    int height = heightmap.getHeight();

    if (width < 3 || height < 3) {
        return;
    }


    float rate = std::clamp(transferRate, 0.0f, 0.5f);

    unsigned availableThreads = std::thread::hardware_concurrency();
    int workerCount = std::min(
        height,
        static_cast<int>(availableThreads == 0 ? 1 : availableThreads)
    );

    int rowsPerWorker = (height + workerCount - 1) / workerCount;
    std::vector<std::thread> workers;
    workers.reserve(workerCount);
    

    std::vector<float> nextValues(values.size());
    std::vector<float> outflow(values.size(), 0.0f);
    std::vector<int> destination(values.size());

    std::barrier outflowBarrier(workerCount);
    std::barrier iterationBarrier(workerCount, [&]() noexcept {
        values.swap(nextValues);
    });

    for (int worker = 0; worker < workerCount; worker++)
    {
        int firstRow = worker * rowsPerWorker;
        int lastRow = std::min(height, firstRow + rowsPerWorker);

        workers.emplace_back([&, firstRow, lastRow]{
            for (int iter = 0; iter < iterations; iter++)
            {
                //pass 1: calc outflow
                calculateOutflow(values, outflow, destination, width, firstRow, lastRow, talusThreshold, rate, height);
                
                //outflow barrier
                outflowBarrier.arrive_and_wait();

                //pass 2: gathers inflow 
                gatherInflow(values, outflow, destination, nextValues, width, firstRow, lastRow, height);

                //swap arrays and barrier
                iterationBarrier.arrive_and_wait();
            }
        });
    }      
    

    for (std::thread& worker : workers) {
        worker.join();
    }
}
