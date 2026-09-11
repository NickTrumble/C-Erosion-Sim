#include "colour_map.hpp"

ColourMapType ColourMap::nextColourMap(ColourMapType current){
    int val = static_cast<int>(current);
    int total = static_cast<int>(ColourMapType::Count);

    return static_cast<ColourMapType>((val + 1) % total);
}