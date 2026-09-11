#pragma once

#include "input.hpp" 

class Keybindings{
    public:
        static bool shouldRegenerate(Input& input);
        static bool shouldToggleControls(Input& input);
        static bool shouldChangeColourMap(Input& input);
};