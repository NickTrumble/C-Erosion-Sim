#include "benchmark.hpp"

#include <chrono>
#include <iostream>
#include <string>

namespace {

using Clock = std::chrono::steady_clock;

Clock::time_point startTimer() {
    return Clock::now();
}

double endTimer(const Clock::time_point& startPoint, const std::string& name) {
    const double duration =
        std::chrono::duration<double, std::milli>(Clock::now() - startPoint).count();

    std::cout << name << ": " << duration << "ms\n";
    return duration;
}

} // namespace

void runHeightmapBenchmark(TerrainGeneratorSettings noise, float scale, int attempts) {
    std::string benchmarkName = "Heightmap Generation";

    if (attempts <= 0) {
        return;
    }

    double totalDuration = 0.0;

    for (int attempt = 0; attempt < attempts; ++attempt) {
        const auto start = startTimer();
        Heightmap heightmap = TerrainGenerator::generate(noise, scale);
        totalDuration += endTimer(start, benchmarkName);
    }

    std::cout << benchmarkName << " average: "
              << totalDuration / attempts << "ms\n";
}
