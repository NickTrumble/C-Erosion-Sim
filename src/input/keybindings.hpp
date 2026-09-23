#pragma once

#include "input.hpp" 

class Keybindings{
    public:
        static bool shouldRegenerate(Input& input);
        static bool shouldAnimate(Input& input);
        static bool shouldBenchmark(Input& input);
        static bool shouldToggleControls(Input& input);
        static bool shouldChangeColourMap(Input& input);
        static bool shouldSave(Input& input);
        static bool shouldLoad(Input& input);
};
