#include "thermal_erosion.hpp"

#include <algorithm>
#include <array>
#include <thread>
#include <vector>
#include <barrier>

void ThermalErosion::calculateChangesForRows(
    std::vector<float>& values,
    std::vector<float>& localChanges,
    int width,
    int firstRow,
    int lastRow,
    float talusThreshold,
    float transferRate
) {
    static std::array<std::array<int, 2>, 4> neighbourOffsets{{
        {{-1, 0}},
        {{ 1, 0}},
        {{ 0,-1}},
        {{ 0, 1}}
    }};

    for (int y = firstRow; y < lastRow; ++y) {
        for (int x = 1; x < width - 1; ++x) {
            int currentIndex = y * width + x;
            float currentHeight = values[currentIndex];

            float steepestDrop = talusThreshold;
            int lowestX = x;
            int lowestY = y;

            for ( auto& offset : neighbourOffsets) {
                int neighbourX = x + offset[0];
                int neighbourY = y + offset[1];
                float neighbourHeight = values[neighbourY * width + neighbourX];
                float drop = currentHeight - neighbourHeight;

                if (drop > steepestDrop) {
                    steepestDrop = drop;
                    lowestX = neighbourX;
                    lowestY = neighbourY;
                }
            }

            if (lowestX == x && lowestY == y) {
                continue;
            }

            float movedHeight = (steepestDrop - talusThreshold) * transferRate;
            localChanges[currentIndex] -= movedHeight;
            localChanges[lowestY * width + lowestX] += movedHeight;
        }
    }
}

void ThermalErosion::combineWorkerChanges(
    std::vector<std::vector<float>>& workerChanges,
    std::vector<float>& heightChanges
) {
    std::fill(heightChanges.begin(), heightChanges.end(), 0.0f);

    for (auto& localChanges : workerChanges) {
        for (int index = 0; index < static_cast<int>(heightChanges.size()); ++index) {
            heightChanges[index] += localChanges[index];
        }
    }
}

void ThermalErosion::applyHeightChanges(
    std::vector<float>& values,
    std::vector<float>& heightChanges
) {
    for (int index = 0; index < static_cast<int>(values.size()); ++index) {
        values[index] += heightChanges[index];
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

    std::vector<float> heightChanges(width * height);

    unsigned availableThreads = std::thread::hardware_concurrency();
    int workerCount = std::min(
        height - 2,
        static_cast<int>(availableThreads == 0 ? 1 : availableThreads)
    );

    int rowsPerWorker = (height - 2 + workerCount - 1) / workerCount;
    std::vector<std::thread> workers;
    workers.reserve(workerCount);

    std::vector<std::vector<float>> workerChanges(workerCount, std::vector<float>(width * height, 0.0f));

    std::barrier barrier(workerCount, [&]() noexcept {
        combineWorkerChanges(workerChanges, heightChanges);
        applyHeightChanges(values, heightChanges);
    });

    for (int worker = 0; worker < workerCount; worker++){
        int firstRow = 1 + worker * rowsPerWorker;
        int lastRow = std::min(height - 1, firstRow + rowsPerWorker);

        workers.emplace_back([&, worker, firstRow, lastRow]{
            auto& localChanges = workerChanges[worker];

            for (int iteration = 0; iteration < iterations; iteration++){
                std::fill(localChanges.begin(), localChanges.end(), 0.0f);

                calculateChangesForRows(
                    values,
                    localChanges,
                    width,
                    firstRow,
                    lastRow,
                    talusThreshold,
                    rate
                );

                barrier.arrive_and_wait();
            }
        });
    }

    for (std::thread& worker : workers) {
        worker.join();
    }

    // for (int iter = 0; iter < iterations; ++iter) {
    //     for (auto& localChanges : workerChanges) {
    //         std::fill(localChanges.begin(), localChanges.end(), 0.0f);
    //     }

    //     for (int worker = 0; worker < workerCount; ++worker) {
    //         int firstRow = 1 + worker * rowsPerWorker;
    //         int lastRow = std::min(height - 1, firstRow + rowsPerWorker);

    //         auto& localChanges = workerChanges[worker];

    //         workers.emplace_back([
    //             firstRow,
    //             lastRow,
    //             talusThreshold,
    //             rate,
    //             &localChanges,
    //             width,
    //             &values
    //         ] {
    //             ThermalErosion::calculateChangesForRows(
    //                 values,
    //                 localChanges,
    //                 width,
    //                 firstRow,
    //                 lastRow,
    //                 talusThreshold,
    //                 rate
    //             );
    //         });
    //     }

    //     for (std::thread& worker : workers) {
    //         worker.join();
    //     }
    //     workers.clear();

    //     ThermalErosion::combineWorkerChanges(workerChanges, heightChanges);
    //     ThermalErosion::applyHeightChanges(values, heightChanges);
    // }
}
