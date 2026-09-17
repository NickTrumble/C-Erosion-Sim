cmake_minimum_required(VERSION 3.20)

project(TerrainGenerator)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(TerrainGenerator
    src/main.cpp
    src/benchmark/benchmark.cpp
    src/render/render.cpp
    src/render/camera.cpp
    src/render/window.cpp
    src/terrain/heightmap.cpp
    src/maths/noise/baseNoise.cpp
    src/maths/noise/perlin.cpp
    src/maths/noise/simplex.cpp
    src/terrain/terrain_generator.cpp
    src/input/input.cpp
    src/input/keybindings.cpp
    src/render/colour_map.cpp
    src/render/shaders/shader.cpp
    src/terrain/erosion/thermal_erosion.cpp
    src/terrain/chunk.cpp
)


target_link_libraries(TerrainGenerator PRIVATE glfw glad::glad OpenGL::GL)
target_compile_definitions(TerrainGenerator PRIVATE
    TERRAIN_SHADER_DIR="${CMAKE_CURRENT_SOURCE_DIR}/src/render/shaders"
)
