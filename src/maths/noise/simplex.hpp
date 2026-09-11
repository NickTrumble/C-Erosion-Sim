#pragma once

#include "baseNoise.hpp"

class simplex : public baseNoise
{
    public:
        simplex(int size, int octave, float pers);

        float octave(float x, float y) override;
        
    private:
        float skew;
        float unskew;

        float cornerContribution(float x, float y, int index);
};