#pragma once

#include <vector>
#include "../maths/noise/perlin.hpp"

class Heightmap{
    public:
        Heightmap(int width, int height);

        int getHeight() const;
        int getWidth() const;

        float getHeightAt(int x, int y) const;
        void setHeightAt(int x, int y, float val);

        static Heightmap generateHeightmap(perlin& noise, float scale);
        std::pair<float, float> findHeightRange() const;
        const std::vector<float>& getValues() const {
            return values;
        }
        
    private:
        int width;
        int height;
        std::vector<float> values;

        int index(int x, int y) const;
};