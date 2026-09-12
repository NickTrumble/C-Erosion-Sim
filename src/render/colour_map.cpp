#include "colour_map.hpp"
#include <algorithm>
#include <iostream>
#include <cmath>

using Colour = std::array<unsigned char, 3>;

constexpr Colour black {  0,   0,   0 };
constexpr Colour white {255, 255, 255 };
constexpr std::array<Colour, 2> greyscaleColours {
    black, white
};

// Terrain map
constexpr Colour deepWater { 18,  52,  86 };
constexpr Colour shallowWater { 52, 118, 168 };
constexpr Colour sand {210, 190, 140 };
constexpr Colour grass { 91, 145,  76 };
constexpr Colour forest { 43,  98,  57 };
constexpr Colour rock {125, 116, 105 };
constexpr Colour snow {238, 243, 246 };
constexpr std::array<Colour, 7> terrainColours {
    deepWater, shallowWater, sand, grass, forest, rock, snow
};

// Heat map
constexpr Colour heatBlue { 25,  38, 120 };
constexpr Colour heatCyan { 32, 177, 208 };
constexpr Colour heatYellow {245, 218,  58 };
constexpr Colour heatOrange {239, 119,  35 };
constexpr Colour heatRed {190,  36,  39 };
constexpr std::array<Colour, 5> heatColours {
    heatBlue, heatCyan, heatYellow, heatOrange, heatRed
};

ColourMapType ColourMap::nextColourMap(ColourMapType current){
    int val = static_cast<int>(current);
    int total = static_cast<int>(ColourMapType::Count);

    return static_cast<ColourMapType>((val + 1) % total);
}

int ColourMap::getColourCount(ColourMapType type) {
    switch (type) {
        case ColourMapType::Greyscale:
            return static_cast<int>(greyscaleColours.size());

        case ColourMapType::Terrain:
            return static_cast<int>(terrainColours.size());

        case ColourMapType::Heatmap:
            return static_cast<int>(heatColours.size());

        default:
            return 0;
    }
}

Colour ColourMap::colourToHeight(float height, ColourMapType type, float min, float max){
    int colourCount = getColourCount(type);

    float normal = (height - min) / (max - min);
    float val = normal * (colourCount - 1);

    int lowerBound = static_cast<int>(std::floor(val));
    int upperBound = std::min(lowerBound + 1, colourCount - 1);
 
    return interpolateColour(enumToColour(type, lowerBound), enumToColour(type, upperBound), val);
}

Colour ColourMap::interpolateColour(Colour c1, Colour c2, float val){
    float t = std::clamp(val - static_cast<int>(val), 0.0f, 1.0f);
    return Colour{
        static_cast<unsigned char>(c1[0] + (c2[0] - c1[0]) * t),
        static_cast<unsigned char>(c1[1] + (c2[1] - c1[1]) * t),
        static_cast<unsigned char>(c1[2] + (c2[2] - c1[2]) * t)
    };
}

Colour ColourMap::enumToColour(ColourMapType type, int ordinal){
    switch (type)
    {
    case ColourMapType::Greyscale:
        if (ordinal < 0 || ordinal >= static_cast<int>(greyscaleColours.size())) {
            std::cout<<"error out of array bounds greyscale " << ordinal << "\n";
            return black;
        }
        return greyscaleColours[ordinal];
    case ColourMapType::Heatmap:
        if (ordinal < 0 || ordinal >= static_cast<int>(heatColours.size())) {
            std::cout<<"error out of array bounds heat\n";
            return black;
        }
        return heatColours[ordinal];
    case ColourMapType::Terrain:
    if (ordinal < 0 || ordinal >= static_cast<int>(terrainColours.size())) {
            std::cout<<"error out of array bounds terr\n";
            return black;
        }
        return terrainColours[ordinal];
    default:
        return black;
    }
}

const char* colourMapName(ColourMapType type) {
    switch (type) {
        case ColourMapType::Greyscale: 
            return "Greyscale";
        case ColourMapType::Terrain:   
            return "Terrain";
        case ColourMapType::Heatmap:   
            return "Heatmap";
        default:                       
            return "Unknown";
    }
}