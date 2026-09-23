#include "render/Window.hpp"
#include "benchmark/benchmark.hpp"
#include "terrain/heightmap.hpp"
#include "render/render.hpp"
#include "input/input.hpp"
#include "terrain/terrain_generator.hpp"
#include "render/colour_map.hpp"
#include "render/camera.hpp"
#include "app/app_state.hpp"
#include "input/keybindings.hpp"
#include "terrain/erosion/thermal_erosion.hpp"
#include "app/exportTerrain.hpp"
#include <chrono>
#include <future>
#include <optional>
#include <random>
#include <cmath>
#include <algorithm>
#include <iostream>
#include "app/file_dialog.hpp"

bool handleInputs(Input& input, AppState& state, bool isGenerating){
    if (Keybindings::shouldRegenerate(input)){
        if (isGenerating){
            std::cout << "Terrain generation is already in progress.\n";
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
    bool benchmark = true;

    int windowWidth = 1024;
    int windowHeight = 1024;

    TerrainGeneratorSettings terrainSettings;
    AppState appState;

    Input input;
    Window window(windowWidth, windowHeight, "Terrain generator");

    if (benchmark) {
        runHeightmapBenchmark(terrainSettings);
    }

    Render renderer;
    Camera camera;
    Heightmap heightmap(1, 1);
    renderer.uploadHeightmap(heightmap);

    std::future<Heightmap> pendingHeightmap;
    int erosionIterationsRemaining = 0;
    auto prevTimeFrame = std::chrono::steady_clock::now();

    auto startAnimatedGeneration = [&] {
        const TerrainGeneratorSettings generatorSettings = terrainSettings;

        pendingHeightmap = std::async(
            std::launch::async,
            [generatorSettings] {
                return TerrainGenerator::generateBase(generatorSettings);
            }
        );
    };

    startAnimatedGeneration();

    while (!window.shouldClose()) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        window.pollEvents();
        input.update(window);

        float duration = getTimeDifference(prevTimeFrame);
        camera.update(window, input, duration);
        prevTimeFrame = std::chrono::steady_clock::now();

        const bool isTerrainGenerating =
            pendingHeightmap.valid() || erosionIterationsRemaining > 0;

        if (handleInputs(input, appState, isTerrainGenerating)) {
            terrainSettings.seed = std::random_device{}();
            startAnimatedGeneration();
        }

        //handle saving
        if (Keybindings::shouldSave(input)) {
            if (isTerrainGenerating) {
                std::cout << "Wait for terrain generation to finish before saving.\n";
            } else {
                auto savePath = FileDialog::chooseSavePath();
                ExportTerrain terrainExporter;
                    
                terrainExporter.saveTerrain(
                    heightmap,
                    terrainSettings,
                    *savePath
                );

                std::cout << "Terrain saved to: " << savePath.value().string() << '\n';
            }
        }

        //handle loading
        if (Keybindings::shouldLoad(input)){
            if (isTerrainGenerating){
                std::cout << "Wait for terrain generation to finish before saving.\n";
            }
            else {
                auto loadPath = FileDialog::chooseLoadPath();
                ExportTerrain terrainExporter;
                if (!loadPath){
                    std::cout << "Path choosing unsuccessful\n";
                } else {
                    auto loaded = terrainExporter.loadTerrain(*loadPath);
                    if (!loaded){
                        std::cout << "Terrain loading unsuccessful. \n";
                    } else {
                        heightmap = std::move(loaded.value().heightmap);
                        terrainSettings = loaded.value().settings;
                        erosionIterationsRemaining = 0;

                        renderer.uploadHeightmap(heightmap);

                        std::cout << "Terrain loaded successfully from: " << loadPath.value().string() << ".\n";
                    }
                }
            
                
            }
        }

        bool heightmapChanged = false;
        if (pendingHeightmap.valid() &&
            pendingHeightmap.wait_for(std::chrono::milliseconds{0}) ==
                std::future_status::ready) {
            heightmap = pendingHeightmap.get();
            erosionIterationsRemaining = terrainSettings.erosionIterations;
            heightmapChanged = true;

            std::cout << "Base terrain generated with seed: "<< terrainSettings.seed << "\n";
        }

        if (!heightmapChanged && erosionIterationsRemaining > 0) {
            int erosionSteps = std::clamp(
                terrainSettings.erosionIterationsPerFrame,
                1,
                erosionIterationsRemaining
            );

            ThermalErosion::apply(
                heightmap,
                erosionSteps,
                terrainSettings.talusThreshold,
                terrainSettings.transferRate
            );

            erosionIterationsRemaining -= erosionSteps;
            heightmapChanged = true;
        }

        if (heightmapChanged) {
            renderer.uploadHeightmap(heightmap);
        }

        const std::pair<int, int> frameBufferSize = window.getFramebufferSize();
        renderer.drawHeightmap(frameBufferSize.first, frameBufferSize.second, appState, camera);
        window.swapBuffers();
    }

    return 0;
}
