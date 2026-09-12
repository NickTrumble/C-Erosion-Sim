#include "render/Window.hpp"
#include "benchmark/benchmark.hpp"
#include "maths/noise/perlin.hpp"
#include "terrain/heightmap.hpp"
#include "render/render.hpp"
#include "input/input.hpp"
#include "terrain/terrain_generator.hpp"
#include "render/colour_map.hpp"
#include "app/app_state.hpp"
#include "input/keybindings.hpp"
#include <chrono>
#include <future>
#include <optional>

bool handleInputs(Input& input, AppState& state, bool isGenerating){
    if (Keybindings::shouldRegenerate(input)){
        if (isGenerating){
            std::cout<<"Already generating new map";
        }
        else {
            std::cout << "Regenerated\n";
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

int main() {
    bool benchmark = false;

    int windowWidth = 1024;
    int windowHeight = 1024;
    
    TerrainGeneratorSettings terrainSettings;
    AppState appState;      
    float scale = 0.02;

    Input input;
    Window window(windowWidth, windowHeight, "Terrain generator");
    perlin noise(terrainSettings.heightmapSize, terrainSettings.octaves, terrainSettings.persistence);
    Heightmap heightmap = Heightmap::generateHeightmap(noise, scale);

    if (benchmark) runHeightmapBenchmark(noise, scale);

    Render renderer;
    renderer.uploadHeightmap(heightmap);
    std::optional<std::future<Heightmap>> pendingHeightmap;

    while (!window.shouldClose()){

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        renderer.drawHeightmap(windowWidth, windowHeight, appState);

        window.swapBuffers();
        window.pollEvents();
        input.update(window);


        if (pendingHeightmap.has_value() && pendingHeightmap->wait_for(std::chrono::milliseconds{0}) == std::future_status::ready){
            heightmap = pendingHeightmap->get();
            renderer.uploadHeightmap(heightmap);

            pendingHeightmap.reset();
            std::cout<<"Terrain generation complete\n";
        }

        if (handleInputs(input, appState, pendingHeightmap.has_value())){
            TerrainGeneratorSettings generatorSettings = terrainSettings;

            pendingHeightmap.emplace(std::async(
                std::launch::async,
                [generatorSettings, scale] {
                    perlin generatedNoise(
                        generatorSettings.heightmapSize,
                        generatorSettings.octaves,
                        generatorSettings.persistence
                    );

                    return Heightmap::generateHeightmap(
                        generatedNoise,
                        scale
                    );
                }
            ));

        }
    }

    return 0;
}

