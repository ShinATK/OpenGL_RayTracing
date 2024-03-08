#include "Light.h"

Light::Light(glm::vec3 position, glm::vec3 color, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) :
    m_position(position), m_color(color), m_ambient(ambient), m_diffuse(diffuse), m_specular(specular), m_shown(true), m_size(0.5f), m_VAO(0)
{

}

Light::~Light() {
    glDeleteVertexArrays(1, &m_VAO);
}

void Light::Draw(std::string new_shader)
{
    glm::mat4 model = glm::mat4(1.0f); // 一定记着初始化
    model = glm::translate(model, m_position);
    model = glm::scale(model, m_size);

    ResourceManager::GetShader(new_shader).SetMatrix4("model", model);
    ResourceManager::GetShader(new_shader).SetVector3f("color", m_color);
    ResourceManager::GetShader(new_shader).Use();

    this->BindVAO();
}

void Light::BindVAO()
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

    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    unsigned int VBO, EBO;

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindVertexArray(0);

    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

