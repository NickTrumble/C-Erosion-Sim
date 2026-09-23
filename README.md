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

The timing covers `Heightmap::generateHeightmap()` only. Perlin construction,
window setup, rendering, and display are excluded.

| Run | Time (ms) |
|---:|---:|
| 1 | 281.233 |
| 2 | 268.378 |
| 3 | 262.460 |
| 4 | 280.995 |
| 5 | 287.069 |
| Average | **276.027** |

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

### Thermal erosion

Thermal erosion is used when generating the heightmap through comparing slopes then moving the sediment to the lowest slope nearby, and is applied after default terrain generated.

| Run | Time (ms) |
|---:|---:|
| 1 | 1669.69 |
| 2 | 1978.69 |
| 3 | 1830.25 |
| 4 | 1989.24 |
| 5 | 2549.75 |
| Average | **2003.53** |

This is ***22.28x slower** than the original 89.980 ms average (about a 2228% increase).

### Parallel thermal erosion

Thermal erosion now divides the heightmap into row ranges and calculates each
range on a worker thread. Each worker accumulates changes in its own buffer;
the buffers are combined once all workers finish, avoiding concurrent writes to
the same heightmap cell.

| Run | Time (ms) |
|---:|---:|
| 1 | 1438.37 |
| 2 | 1422.43 |
| 3 | 1425.69 |
| 4 | 1422.50 |
| 5 | 1419.26 |
| Average | **1425.65** |

Workers persist across all erosion iterations and synchronize with a C++20
barrier, eliminating per-iteration thread creation and destruction.

This is a **1.41x speedup** over the earlier 2003.53 ms thermal-erosion
measurement (about a 29% reduction).

### Double-buffered gather erosion

The worker-local full-heightmap buffers and serial reduction were replaced with
a double-buffered gather update. Each erosion iteration first calculates a
cell's outgoing sediment and destination, then each cell gathers incoming
sediment from its four neighbours into the next heightmap buffer. A barrier
swaps the current and next buffers before the following iteration.

This removes the serial combination of a full change map for every worker.
The reported `Heightmap Generation` measurement covers the complete
`TerrainGenerator::generate()` call, including heightmap generation and
thermal erosion.

| Run | Time (ms) |
|---:|---:|
| 1 | 737.262 |
| 2 | 856.450 |
| 3 | 688.822 |
| 4 | 730.781 |
| 5 | 588.396 |
| Average | **720.342** |

This is a **1.98x speedup** over the worker-local-buffer implementation's
1425.65 ms average (about a **49.5% reduction** in end-to-end terrain
generation time).

## Asynchronous regeneration

Pressing `R` starts base-heightmap generation on a background task. The existing
terrain remains visible while worker threads produce the replacement Perlin map.
Once it is ready, the main thread uploads that raw map, then applies thermal
erosion in small batches across subsequent frames. Each updated heightmap is
uploaded before drawing, so the erosion process is visible on screen.

OpenGL calls remain on the main thread because the GLFW OpenGL context belongs to
that thread. Pressing `R` while base generation or erosion is already running
leaves the current task in progress rather than starting a second one.
