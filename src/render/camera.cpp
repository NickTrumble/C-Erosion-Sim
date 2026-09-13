#include "camera.hpp"

#include <algorithm>
#include <cmath>

namespace {
constexpr float MinZoom = 1.0f;
constexpr float MaxZoom = 32.0f;
constexpr float ZoomFactorPerScrollStep = 1.2f;
}

void Camera::update(Window& window, const Input& input) {
    const auto [viewportWidth, viewportHeight] = window.getWindowSize();
    const auto [cursorX, cursorY] = window.getCursorPosition();

    if (input.wasPressed(GLFW_KEY_HOME)) {
        reset();
    }

    const double scrollDelta = window.consumeScrollDelta();
    if (scrollDelta != 0.0 && viewportWidth > 0 && viewportHeight > 0) {
        zoomAt(
            static_cast<float>(cursorX),
            static_cast<float>(cursorY),
            viewportWidth,
            viewportHeight,
            static_cast<float>(scrollDelta)
        );
    }

    if (hasPreviousCursor && window.isMouseButtonDown(GLFW_MOUSE_BUTTON_MIDDLE) &&
        viewportWidth > 0 && viewportHeight > 0) {
        const float deltaX = static_cast<float>(cursorX - previousCursorX);
        const float deltaY = static_cast<float>(cursorY - previousCursorY);

        centerX -= deltaX / (static_cast<float>(viewportWidth) * zoom);
        centerY += deltaY / (static_cast<float>(viewportHeight) * zoom);
        clampToHeightmap();
    }

    previousCursorX = cursorX;
    previousCursorY = cursorY;
    hasPreviousCursor = true;
}

float Camera::getCenterX() const {
    return centerX;
}

float Camera::getCenterY() const {
    return centerY;
}

float Camera::getZoom() const {
    return zoom;
}

void Camera::reset() {
    centerX = 0.5f;
    centerY = 0.5f;
    zoom = MinZoom;
}

void Camera::zoomAt(float cursorX, float cursorY, int viewportWidth, int viewportHeight, float amount) {
    const float cursorUvX = cursorX / static_cast<float>(viewportWidth);
    const float cursorUvY = 1.0f - cursorY / static_cast<float>(viewportHeight);

    const float heightmapX = centerX + (cursorUvX - 0.5f) / zoom;
    const float heightmapY = centerY + (cursorUvY - 0.5f) / zoom;

    zoom = std::clamp(zoom * std::pow(ZoomFactorPerScrollStep, amount), MinZoom, MaxZoom);

    centerX = heightmapX - (cursorUvX - 0.5f) / zoom;
    centerY = heightmapY - (cursorUvY - 0.5f) / zoom;
    clampToHeightmap();
}

void Camera::clampToHeightmap() {
    const float halfVisibleRange = 0.5f / zoom;
    centerX = std::clamp(centerX, halfVisibleRange, 1.0f - halfVisibleRange);
    centerY = std::clamp(centerY, halfVisibleRange, 1.0f - halfVisibleRange);
}
