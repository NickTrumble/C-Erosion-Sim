#include "baseNoise.hpp"
#include <numeric>
#include <random>


baseNoise::baseNoise(int size, int octave, float pers, unsigned int seed):
    numSamples(size),
    octaves(octave),
    persistence(pers),
    seed(seed)
    {
        pTable.resize(size * 2);
        std::iota(pTable.begin(), pTable.begin() + size, 0);

        std::mt19937 generator(seed);

        std::shuffle(pTable.begin(), pTable.begin() + size, generator);

        std::copy(
            pTable.begin(),
            pTable.begin() + size,
            pTable.begin() + size
        );


    }

int baseNoise::fastFloor(float x){
    return (x >= 0) ? (int)x : (int)x - 1;
}

float baseNoise::noiseMethod(float x, float y){
    float amplitude = 1;
    float freq = 1;
    float noise = 0;
    float max = 0;
    for (int i = 0; i < octaves; i++){
        noise += octave(x * freq, y * freq) * amplitude;
        max += amplitude;
        amplitude *= persistence;
        freq *= 2;
    }
    return noise / max;
}

float baseNoise::dotProduct(std::array<float, 2>& gradient, float x, float y){
    return gradient[0] * x + gradient[1] * y;
}

float baseNoise::gradientCalc(int corner, float x, float y){
    return dotProduct(gradients[corner % gradients.size()], x, y);
}

int baseNoise::getSize() const{
    return numSamples;
}