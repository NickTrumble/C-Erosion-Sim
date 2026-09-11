#include "render/Window.hpp"
#include "maths/noise/perlin.hpp"
#include "terrain/heightmap.hpp"
#include "render/render.hpp"
#include <iostream>
#include <chrono>

using Clock = std::chrono::steady_clock;

Clock::time_point startTimer(){
    return Clock::now();
}

double endTimer(Clock::time_point startPoint, std::string name){
    double duration = 
     std::chrono::duration<double, std::milli>(Clock::now() - startPoint).count();
    
    std::cout<< name << ": " << duration << "ms\n";
    return duration;
}

int main() {
    int windowWidth = 1024;
    int windowHeight = 1024;
    
    int heightmapSize = 512;
    int octaves = 6;
    float persistence = 0.5;
    float scale = 0.02;

    Window window(windowWidth, windowHeight, "Terrain generator");

    perlin noise(heightmapSize, octaves, persistence);
    Heightmap heightmap = Heightmap::generateHeightmap(noise, scale);

    //benchmarking
    int benchmarkAttempts = 5;
    double duration = 0;
    std::string benchmarkName = "Heightmap Generation";

    for (int i = 0; i < benchmarkAttempts; i++){
        auto start = startTimer();
        Heightmap h = Heightmap::generateHeightmap(noise, scale);
        duration += endTimer(start, benchmarkName);
    }
    std::cout << benchmarkName << " average: " << duration / benchmarkAttempts << "ms\n";

    Render renderer;

    while (!window.shouldClose()){

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        renderer.drawHeightmap(heightmap, windowWidth, windowHeight);

        window.swapBuffers();
        window.pollEvents();
    }

    return 0;
}

