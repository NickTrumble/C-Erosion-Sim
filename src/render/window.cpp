#include "window.hpp"

#include <iostream>
#include <stdexcept>

Window::Window(int width, int height, const std::string& title){
    if (!glfwInit()){
        throw std::runtime_error("failed to initialise glfw");
    }

    window = glfwCreateWindow(
        width,
        height,
        title.c_str(),
        nullptr,
        nullptr
    );

    if (!window){
        glfwTerminate();
        throw std::runtime_error("failed to initialise window");
    }

    glfwMakeContextCurrent(window);

    glfwSetWindowUserPointer(window, this);
    glfwSetScrollCallback(window, Window::scrollCallback);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))){
        throw std::runtime_error("Failed to load glad opelgl");
    }
}

Window::~Window()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

bool Window::shouldClose() const {
    return glfwWindowShouldClose(window);
}

bool Window::isKeyDown(int key) const{
    return glfwGetKey(window, key) == GLFW_PRESS;
}

bool Window::isMouseButtonDown(int button) const {
    return glfwGetMouseButton(window, button) == GLFW_PRESS;
}

std::pair<double, double> Window::getCursorPosition() const {
    double x = 0.0;
    double y = 0.0;
    glfwGetCursorPos(window, &x, &y);
    return {x, y};
}

std::pair<int, int> Window::getWindowSize() const {
    int width = 0;
    int height = 0;
    glfwGetWindowSize(window, &width, &height);
    return {width, height};
}

std::pair<int, int> Window::getFramebufferSize() const {
    int width = 0;
    int height = 0;
    glfwGetFramebufferSize(window, &width, &height);
    return {width, height};
}

double Window::consumeScrollDelta() {
    const double result = scrollDelta;
    scrollDelta = 0.0;
    return result;
}

void Window::scrollCallback(GLFWwindow* glfwWindow, double, double yOffset) {
    auto* owner = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
    if (owner) {
        owner->scrollDelta += yOffset;
    }
}

void Window::pollEvents() {
    glfwPollEvents();
}

void Window::swapBuffers(){
    glfwSwapBuffers(window);
}
