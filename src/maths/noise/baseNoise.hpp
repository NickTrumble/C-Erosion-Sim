#pragma once

#include <vector>
#include <array>

class baseNoise
{
    public:
        baseNoise(int size, int octave, float pers);

        virtual float octave(float x, float y) = 0;
        float noiseMethod(float x, float y);
        int getSize() const;

    protected:
        std::vector<int> pTable;
        int numSamples;
        int octaves;
        float persistence;

        int fastFloor(float x);
        float gradientCalc(int corner, float x, float y);
        float dotProduct(std::array<float, 2>& gradient, float x, float y);

    private:
        std::array<std::array<float, 2>, 8> gradients{{
            {{ 1.0f,  1.0f}},
            {{-1.0f,  1.0f}},
            {{ 1.0f, -1.0f}},
            {{-1.0f, -1.0f}},
            {{ 1.0f,  0.0f}},
            {{-1.0f,  0.0f}},
            {{ 0.0f,  1.0f}},
            {{ 0.0f, -1.0f}}
        }};

};
