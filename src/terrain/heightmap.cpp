#include "heightmap.hpp"
#include <algorithm>
#include <thread>
#include <vector>

Heightmap::Heightmap(int width, int height):
    width(width),
    height(height),
    values(width * height, 0.0f)
    {}

int Heightmap::getWidth() const {
    return width;
}

int Heightmap::getHeight() const {
    return height;
}

float Heightmap::getHeightAt(int x, int y) const{
    return values[index(x,y)];
}

void Heightmap::setHeightAt(int x, int y, float val){
    values[index(x, y)] = val;
}

int Heightmap::index(int x, int y) const{
    return y * width + x;
}

Heightmap Heightmap::generateHeightmap(perlin& noise, float scale){
    int size = noise.getSize();
    Heightmap heightmap(size, size);

    unsigned availableThreads = std::thread::hardware_concurrency();
    int workerCount = std::min(
        size,
        static_cast<int>(availableThreads == 0 ? 1 : availableThreads)
    );

    int rowsPerWorker = (size + workerCount - 1) / workerCount;
    std::vector<std::thread> workers;
    workers.reserve(workerCount);

    for (int worker = 0; worker < workerCount; worker++) {
        int firstRow = worker * rowsPerWorker;
        int lastRow = std::min(size, firstRow + rowsPerWorker);

        workers.emplace_back([
            &heightmap,
            &noise,
            scale,
            size,
            firstRow,
            lastRow
        ] {
            for (int y = firstRow; y < lastRow; ++y) {
                for (int x = 0; x < size; ++x) {
                    heightmap.values[y * size + x] = noise.noiseMethod(
                        x * scale,
                        y * scale
                    );
                }
            }
        });
    }

    for (std::thread& worker : workers) {
        worker.join();
    }

    return heightmap;
}

std::pair<float, float> Heightmap::findHeightRange() const{
    float min = 10;
    float max = -10;
    float val = 0;
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            val = getHeightAt(i, j);
            if (val > max){
                max = val;
            } else if (val < min){
                min = val;
            }
        }
    }
    return std::pair<float, float>(min, max);
}

std::vector<float>& Heightmap::getValues(){
    return values;
}

const std::vector<float>& Heightmap::getValues() const{
    return values;
}
