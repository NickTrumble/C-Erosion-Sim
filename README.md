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
