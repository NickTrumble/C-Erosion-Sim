#include "keybindings.hpp"

//R - REGENERATE TERRAIN WITH ANIMATION
//LSHIFT - SHOW CONTROLS OVERLAY
//C - CHANGE TERRAIN COLOUR MAP

bool Keybindings::shouldRegenerate(Input& input){
    return input.wasPressed(GLFW_KEY_R);
}

bool Keybindings::shouldToggleControls(Input& input){
    return input.wasPressed(GLFW_KEY_LEFT_SHIFT);
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