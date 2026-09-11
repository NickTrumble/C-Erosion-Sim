#pragma once

#include "../render/colour_map.hpp"

struct AppState{
    bool showControls = true;
    ColourMapType type = ColourMapType::Greyscale;
};

