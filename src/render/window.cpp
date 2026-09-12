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

void Window::pollEvents() {
    glfwPollEvents();
}

void Window::swapBuffers(){
    glfwSwapBuffers(window);
}