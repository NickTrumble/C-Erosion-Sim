#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <utility>

class Window{
    public:
        Window(int width, int height, const std::string& title);
        ~Window();

        bool shouldClose() const;
        bool isKeyDown(int key) const;
        bool isMouseButtonDown(int button) const;
        std::pair<double, double> getCursorPosition() const;
        std::pair<int, int> getWindowSize() const;
        std::pair<int, int> getFramebufferSize() const;
        double consumeScrollDelta();
        void pollEvents();
        void swapBuffers();

    private:
        static void scrollCallback(GLFWwindow* glfwWindow, double xOffset, double yOffset);

        GLFWwindow* window;
        double scrollDelta = 0.0;
};
