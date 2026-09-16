#include "render/Window.hpp"
#include "benchmark/benchmark.hpp"
#include "maths/noise/perlin.hpp"
#include "terrain/heightmap.hpp"
#include "render/render.hpp"
#include "input/input.hpp"
#include "terrain/terrain_generator.hpp"
#include "render/colour_map.hpp"
#include "render/camera.hpp"
#include "app/app_state.hpp"
#include "input/keybindings.hpp"
#include <chrono>
#include <future>
#include <optional>
#include <random>
#include <cmath>

bool handleInputs(Input& input, AppState& state, bool isGenerating){
    if (Keybindings::shouldRegenerate(input)){
        if (isGenerating){
            std::cout<<"Already generating new map";
        }
        else {
            return true;
        }
    }
    if (Keybindings::shouldToggleControls(input)){
        state.showControls = !state.showControls;
        std::cout<<"Controls changed \n";
    }
    if (Keybindings::shouldChangeColourMap(input)){
        state.type = ColourMap::nextColourMap(state.type);
        std::cout <<"Colour changed: " << colourMapName(state.type) << "\n";
    }
    return false;
}

float getTimeDifference(std::chrono::steady_clock::time_point previous){
    return std::chrono::duration<float>(
        std::chrono::steady_clock::now() - previous
    ).count();
}

int main() {
    bool benchmark = false;

    int windowWidth = 1024;
    int windowHeight = 1024;
    
    TerrainGeneratorSettings terrainSettings;
    AppState appState;      
    float scale = 0.02;

    Input input;
    Window window(windowWidth, windowHeight, "Terrain generator");
    perlin noise(terrainSettings.heightmapSize, terrainSettings.octaves, terrainSettings.persistence, terrainSettings.seed);
    Heightmap heightmap = TerrainGenerator::generate(terrainSettings, scale);

    if (benchmark) runHeightmapBenchmark(noise, scale);

    Render renderer;
    Camera camera;
    renderer.uploadHeightmap(heightmap);
    std::future<Heightmap> pendingHeightmap;
    std::chrono::steady_clock::time_point prevTimeFrame;
    float duration;

    while (!window.shouldClose()){

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        window.pollEvents();
        input.update(window);

        duration = getTimeDifference(prevTimeFrame);
        camera.update(window, input, duration);
        prevTimeFrame = std::chrono::steady_clock::now();

        std::pair<int, int> frameBufferSize = window.getFramebufferSize();
        renderer.drawHeightmap(frameBufferSize.first, frameBufferSize.second, appState, camera);

        window.swapBuffers();


        if (pendingHeightmap.valid() && pendingHeightmap.wait_for(std::chrono::milliseconds{0}) == std::future_status::ready){
            heightmap = pendingHeightmap.get();
            renderer.uploadHeightmap(heightmap);

            std::cout<<"Terrain generated with seed: " << terrainSettings.seed <<"\n";
        }

        if (handleInputs(input, appState, pendingHeightmap.valid())){
            TerrainGeneratorSettings& generatorSettings = terrainSettings;

            generatorSettings.seed = std::random_device{}();

            pendingHeightmap = std::async(
                std::launch::async,
                [generatorSettings, scale] {
                    
                    return TerrainGenerator::generate(
                        generatorSettings,
                        scale
                    );
                }
            );

        }
        
    }

    return 0;
}
