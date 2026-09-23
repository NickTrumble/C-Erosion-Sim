#include "keybindings.hpp"

//R - REGENERATE TERRAIN WITHOUT ANIMATIONS
//A - REGENERATE TERRAIN WITH ANIMATION
//B - BENCHMARK COMPLETE TERRAIN GENERATION
//K - SHOW CONTROLS OVERLAY
//C - CHANGE TERRAIN COLOUR MAP

bool Keybindings::shouldRegenerate(Input& input){
    return input.wasPressed(GLFW_KEY_R);
}

bool Keybindings::shouldAnimate(Input& input){
    return input.wasPressed(GLFW_KEY_A);
}

bool Keybindings::shouldBenchmark(Input& input){
    return input.wasPressed(GLFW_KEY_B);
}

bool Keybindings::shouldToggleControls(Input& input){
    return input.wasPressed(GLFW_KEY_K);
}

bool Keybindings::shouldChangeColourMap(Input& input){
    return input.wasPressed(GLFW_KEY_C);
}

bool Keybindings::shouldSave(Input& input){
    return input.wasPressed(GLFW_KEY_S);
}

bool Keybindings::shouldLoad(Input& input){
    return input.wasPressed(GLFW_KEY_L);
}
