#pragma once

#include <array>

enum class ColourMapType {
    Greyscale,
    Terrain,
    Heatmap,
    Count
};

class ColourMap{
    public:
        static std::array<unsigned char, 3> colourToHeight(
            float height,
            ColourMapType type
        );

        static ColourMapType nextColourMap(ColourMapType current);
};