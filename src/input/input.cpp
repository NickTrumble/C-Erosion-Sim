#include "input.hpp"
#include "../render/window.hpp"


//update arrays for previous and current keys
void Input::update(Window& window){
    previousKeys = currentKeys;

    for (int i = 0; i <= GLFW_KEY_LAST; i++){
        currentKeys[i] = window.isKeyDown(i);
    }
}

bool Input::wasPressed(int key) const{
    return currentKeys[key] && !previousKeys[key];
}