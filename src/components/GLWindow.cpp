#include "GLWindow.h"

GLWindow::GLWindow(int width, int height, const char* title) : m_width(width), m_height(height), m_title(title), m_window(nullptr) {}

GLWindow::~GLWindow()
{
    // ���� GLFW �����Դ
    glfwTerminate();
}

bool GLWindow::Init() {
    // ��ʼ�� GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }

    // ��������
    m_window = glfwCreateWindow(m_width, m_height, m_title, NULL, NULL);
    if (!m_window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    // ���õ�ǰ������
    glfwMakeContextCurrent(m_window);

    // ��ʼ�� GLAD������ OpenGL ����ָ��
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    // �����ӿڴ�С
    glViewport(0, 0, m_width, m_height);

    return true;
}

void GLWindow::Update()
{
    // ��鴰���¼�
    glfwPollEvents();
    // ����������
    glfwSwapBuffers(m_window);
}

bool GLWindow::ShouldClose()
{
    // ��鴰���Ƿ�Ӧ�ùر�
    return glfwWindowShouldClose(m_window);
}

GLFWwindow* GLWindow::GetWindow() const
{
    return this->m_window;
}


