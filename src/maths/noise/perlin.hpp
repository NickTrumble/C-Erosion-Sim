#pragma once

#include "baseNoise.hpp"

class perlin : public baseNoise
{
    public:
        perlin(int size, int octave, float pers, unsigned int seed);

        float octave(float x, float y) override;
        
    private:
        float lerp(float t, float a, float b) const;
        float fade(float f) const;
};