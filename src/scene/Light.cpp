#include "Light.h"

Light::Light() : m_position(0.0f), m_color(1.0f), m_shown(false)
{

}

Light::~Light()
{
    //
}

void Light::Show()
{
	this->m_shown = true;
}

void Light::Hide()
{
	this->m_shown = false;
}

void Light::SetPosition(glm::vec3 position)
{
	this->m_position = position;
}

void Light::SetPosition(float x, float y, float z)
{
	this->m_position = glm::vec3(x, y, z);
}

void Light::SetColor(glm::vec3 color)
{
	this->m_color = color;
}

void Light::SetColor(float R, float G, float B)
{
	this->m_color = glm::vec3(R, G, B);
}

void Light::RenderCube()
{
    if (this->m_shown)
    {
        // 顶点数据
        glm::vec3 vertices[] = {
            glm::vec3(-0.5f, -0.5f,  0.5f),
            glm::vec3(0.5f, -0.5f,  0.5f),
            glm::vec3(0.5f,  0.5f,  0.5f),
            glm::vec3(-0.5f,  0.5f,  0.5f),
            // 后面
            glm::vec3(-0.5f, -0.5f, -0.5f),
            glm::vec3(0.5f, -0.5f, -0.5f),
            glm::vec3(0.5f,  0.5f, -0.5f),
            glm::vec3(-0.5f,  0.5f, -0.5f)
        };
        unsigned int indices[] = {
        0, 1, 2, 2, 3, 0,
        1, 5, 6, 6, 2, 1,
        7, 6, 5, 5, 4, 7,
        4, 0, 3, 3, 7, 4,
        3, 2, 6, 6, 7, 3,
        4, 5, 1, 1, 0, 4
        };

        glm::mat4 model = glm::mat4(1.0f); // 一定记着初始化

        // Translate
        model = glm::translate(model, m_position);

        ResourceManager::GetShader("default").SetMatrix4("model", model);

        ResourceManager::GetShader("default").SetBoolean("bUseTexture", false);
        ResourceManager::GetShader("default").SetVector3f("color", m_color);

        unsigned int VBO, VAO, EBO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // 设置顶点属性指针
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // 渲染立方体
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // 清理
        glBindVertexArray(0);
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }
}

