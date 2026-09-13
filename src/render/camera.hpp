#pragma once

#include "../input/input.hpp"
#include "window.hpp"

class Camera {
public:
    void update(Window& window, const Input& input);

    float getCenterX() const;
    float getCenterY() const;
    float getZoom() const;
    void reset();

private:
    void zoomAt(float cursorX, float cursorY, int viewportWidth, int viewportHeight, float amount);
    void clampToHeightmap();

    float centerX = 0.5f;
    float centerY = 0.5f;
    float zoom = 1.0f;
    double previousCursorX = 0.0;
    double previousCursorY = 0.0;
    bool hasPreviousCursor = false;
};
