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
#include "render/colour_map.hpp"
#include "render/render.hpp"    

void handleInput(Input& input, Heightmap& heightmap, perlin& noise, float scale, AppState& state, TerrainGeneratorSettings terrainSettings, Render& renderer){
    if (Keybindings::shouldRegenerate(input)){
        noise = perlin(terrainSettings.heightmapSize, terrainSettings.octaves, terrainSettings.persistence);
        heightmap = Heightmap::generateHeightmap(noise, scale);
        renderer.uploadHeightmap(heightmap);
        std::cout<<"Regenerated \n";
    }
    if (Keybindings::shouldToggleControls(input)){
        state.showControls = !state.showControls;
        std::cout<<"Controls changed \n";
    }
    if (Keybindings::shouldChangeColourMap(input)){
        state.type = ColourMap::nextColourMap(state.type);
        std::cout <<"Colour changed: " << colourMapName(state.type) << "\n";
    }
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

    while (!window.shouldClose()){

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        renderer.drawHeightmap(windowWidth, windowHeight, appState);

        window.swapBuffers();
        window.pollEvents();
        input.update(window);
        handleInput(input, heightmap, noise, scale, appState, terrainSettings, renderer);
    }

    return 0;
}

