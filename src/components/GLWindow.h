#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

class GLWindow {
public:

    GLWindow(int width, int height, const char* title);

    ~GLWindow();

    bool Init();
    void Update();
    bool ShouldClose();
    GLFWwindow* GetWindow() const;

private:
    GLFWwindow* m_window;
    int m_width, m_height;
    const char* m_title;
};


