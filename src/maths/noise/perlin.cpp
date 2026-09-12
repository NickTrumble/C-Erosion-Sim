#include "perlin.hpp"

perlin::perlin(int size, int octave, float pers, unsigned int seed):
    baseNoise(size, octave, pers, seed)
    {}

float perlin::lerp(float t, float a, float b) const{
    return a + t * (b - a);
}

float perlin::fade(float f) const{
    return f * f * f * (f * (f * 6 - 15) + 10);
}

float perlin::octave(float x, float y){
    int xi = fastFloor(x);
    int yi = fastFloor(y);

    float xf = x - xi;
    float yf = y - yi;

    float i = fade(xf);
    float j = fade(yf);

    int topLeft = pTable[pTable[xi] + yi];
    int topRight = pTable[pTable[xi + 1] + yi];
    int bottomLeft = pTable[pTable[xi] + yi + 1];
    int bottomRight = pTable[pTable[xi + 1] + yi + 1];

    float x1 = lerp(i, gradientCalc(topLeft, xf, yf), gradientCalc(topRight, xf - 1, yf));
    float x2 = lerp(i, gradientCalc(bottomLeft, xf, yf - 1), gradientCalc(bottomRight, xf - 1, yf - 1));

    return lerp(j, x1, x2);
}