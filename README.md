# C-Erosion-Sim

## Benchmarks

### Baseline: Perlin heightmap generation

| Setting | Value |
|---|---|
| Build | Release |
| CPU | Intel Core i5-9400 |
| Heightmap size | 512 x 512 |
| Octaves | 6 |
| Persistence | 0.5 |
| Scale | 0.02 |
| Samples | 5 |
| Average time | **276.027 ms** |

The timing covers `Heightmap::generateHeightmap()` only. Perlin construction,
window setup, rendering, and display are excluded.

| Run | Time (ms) |
|---:|---:|
| 1 | 281.233 |
| 2 | 268.378 |
| 3 | 262.460 |
| 4 | 280.995 |
| 5 | 287.069 |

### Parallel row generation

Heightmap rows are independent, so generation now divides the map into row ranges
and runs those ranges on the available CPU threads.

| Run | Time (ms) |
|---:|---:|
| 1 | 89.364 |
| 2 | 108.788 |
| 3 | 82.749 |
| 4 | 101.596 |
| 5 | 67.401 |
| Average | **89.980** |

This is a **3.07x speedup** over the original 276.027 ms average (about a 67% reduction).

## Asynchronous regeneration

Pressing `R` starts generation on a background task. The existing terrain remains
visible while worker threads produce the replacement heightmap. Once the task is
ready, the main thread uploads the completed heightmap to the OpenGL texture.

OpenGL calls remain on the main thread because the GLFW OpenGL context belongs to
that thread. Pressing `R` while a generation task is already running leaves the
current task in progress rather than starting a second one.
