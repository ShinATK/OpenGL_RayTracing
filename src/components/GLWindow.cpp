#include "GLWindow.h"

GLWindow::GLWindow(int width, int height, const char* title) : m_width(width), m_height(height), m_title(title), m_window(nullptr) {}

GLWindow::~GLWindow()
{
    // 清理 GLFW 相关资源
    glfwTerminate();
}

bool GLWindow::Init() {
    // 初始化 GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }

    // 创建窗口
    m_window = glfwCreateWindow(m_width, m_height, m_title, NULL, NULL);
    if (!m_window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    // 设置当前上下文
    glfwMakeContextCurrent(m_window);

    // 初始化 GLAD，加载 OpenGL 函数指针
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    // 设置视口大小
    glViewport(0, 0, m_width, m_height);

    return true;
}

void GLWindow::Update()
{
    // 检查窗口事件
    glfwPollEvents();
    // 交换缓冲区
    glfwSwapBuffers(m_window);
}

bool GLWindow::ShouldClose()
{
    // 检查窗口是否应该关闭
    return glfwWindowShouldClose(m_window);
}

GLFWwindow* GLWindow::GetWindow() const
{
    return this->m_window;
}


