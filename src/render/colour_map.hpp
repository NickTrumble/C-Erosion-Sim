#pragma once

#include <array>

enum class ColourMapType {
    Greyscale,
    Terrain,
    Heatmap,
    Count
};

const char* colourMapName(ColourMapType type);

class ColourMap{
    public:
        static std::array<unsigned char, 3> colourToHeight(float height, ColourMapType type, float min, float max);

        static ColourMapType nextColourMap(ColourMapType current);
    private:
        static std::array<unsigned char, 3> interpolateColour(
            std::array<unsigned char, 3> c1,
            std::array<unsigned char, 3> c2,
            float val
        );
        static std::array<unsigned char, 3> enumToColour(ColourMapType type, int ordinal);
        static int getColourCount(ColourMapType type);
};