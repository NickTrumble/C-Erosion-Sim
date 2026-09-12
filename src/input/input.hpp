#pragma once

#include <array>
#include "../render/window.hpp"


class Input{
    public:
        void update(Window& window);
        bool wasPressed(int key) const;

    private:
        std::array<bool, GLFW_KEY_LAST + 1> currentKeys {};
        std::array<bool, GLFW_KEY_LAST + 1> previousKeys {};
};