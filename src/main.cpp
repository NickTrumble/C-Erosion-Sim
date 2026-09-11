#include "render/Window.hpp"
#include "maths/noise/perlin.hpp"
#include "terrain/heightmap.hpp"
#include "render/render.hpp"
#include <iostream>

int main() {
    Window window(1000, 800, "Terrain generator");

    perlin noise(1000, 6, 0.5);
    Heightmap heightmap = Heightmap::generateHeightmap(noise, 0.02f);
    Render renderer;

    while (!window.shouldClose()){

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        renderer.drawHeightmap(heightmap, 1000, 800);

        window.swapBuffers();
        window.pollEvents();
    }

    return 0;
}

