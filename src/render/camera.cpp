#include "camera.hpp"

#include <algorithm>
#include <cmath>

namespace {
constexpr float MinZoom = 1.0f;
constexpr float MaxZoom = 32.0f;
constexpr float ZoomFactorPerScrollStep = 1.2f;
}

void Camera::update(Window& window, const Input& input, float deltaTime){
    auto [viewportWidth, viewportHeight] = window.getWindowSize();
    auto [cursorX, cursorY] = window.getCursorPosition();

    if (input.wasPressed(GLFW_KEY_HOME)) {
        reset();
    }

    double scrollDelta = window.consumeScrollDelta();
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
        float deltaX = static_cast<float>(cursorX - previousCursorX);
        float deltaY = static_cast<float>(cursorY - previousCursorY);

        targetCenterX -= deltaX / (static_cast<float>(viewportWidth) * targetZoom);
        targetCenterY += deltaY / (static_cast<float>(viewportHeight) * targetZoom);
        clampToHeightmap();
    }

    previousCursorX = cursorX;
    previousCursorY = cursorY;
    hasPreviousCursor = true;

    float amount = 1.0f - std::exp(-smoothness * deltaTime);

    centerX += (targetCenterX - centerX) * amount;
    centerY += (targetCenterY - centerY) * amount;
    zoom += (targetZoom - zoom) * amount;
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
    targetCenterX = 0.5f;
    targetCenterY = 0.5f;
    targetZoom = MinZoom;
}

void Camera::zoomAt(float cursorX, float cursorY, int viewportWidth, int viewportHeight, float amount) {
    float cursorUvX = cursorX / static_cast<float>(viewportWidth);
    float cursorUvY = 1.0f - cursorY / static_cast<float>(viewportHeight);

    float heightmapX = targetCenterX + (cursorUvX - 0.5f) / targetZoom;
    float heightmapY = targetCenterY + (cursorUvY - 0.5f) / targetZoom;

    targetZoom = std::clamp(targetZoom * std::pow(ZoomFactorPerScrollStep, amount), MinZoom, MaxZoom);

    targetCenterX = heightmapX - (cursorUvX - 0.5f) / targetZoom;
    targetCenterY = heightmapY - (cursorUvY - 0.5f) / targetZoom;
    clampToHeightmap();
}

void Camera::clampToHeightmap() {
    float halfVisibleRange = 0.5f / targetZoom;
    targetCenterX = std::clamp(targetCenterX, halfVisibleRange, 1.0f - halfVisibleRange);
    targetCenterY = std::clamp(targetCenterY, halfVisibleRange, 1.0f - halfVisibleRange);
}
