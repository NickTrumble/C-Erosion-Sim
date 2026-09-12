#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

class Window{
    public:
        Window(int width, int height, const std::string& title);
        ~Window();

        bool shouldClose() const;
        bool isKeyDown(int key) const;
        void pollEvents();
        void swapBuffers();

    private:
        GLFWwindow* window;
};