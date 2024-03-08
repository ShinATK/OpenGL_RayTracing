#include "Object.h"

// Constructor(s)

Object::Object(const char* name, std::string texture, GLboolean bUseTexture, glm::vec3 pos, glm::vec3 size) :
    m_name(name),
    m_position(pos), // 默认位置为原点
    m_size(size), // 默认尺寸为单位大小
    m_yaw(0.0f), m_pitch(0.0f), m_roll(0.0f),
    m_model(1.0f),
    m_color(1.0f), // 默认颜色为白色
    m_shown(true), // 默认显示对象
    m_clicked(false), // 默认未选中
    m_bShowBox(true),
    m_VAO(0), // 默认VAO为0
    lightMethod(0),
    m_ambient(1.0f, 0.5f, 0.31f), m_diffuse(1.0f, 0.5f, 0.31f), m_specular(0.5f), m_shininess(32.0f),
    m_shader("default"), // 默认着色器名称为空
    m_bUseTexture(bUseTexture),
    m_texture(texture), // 默认纹理名称为空
    m_bBoxMin(1.0f),
    m_bBoxMax(0.0f),
    m_depth(1.0f)
{
    glGenVertexArrays(1, &m_VAO);
    this->BindVAO();
}


Object::~Object()
{
    //
    glDeleteVertexArrays(1, &m_VAO);
}


// Draw

void Object::Draw(std::string new_shader) {

    if(m_shader != new_shader) m_shader = new_shader;

    m_model = glm::mat4(1.0f);                                                        // 初始化
    m_model = glm::translate(m_model, m_position);                                    // Translate
    m_model = glm::rotate(m_model, glm::radians(m_yaw), glm::vec3(1, 0, 0));// Rotation
    m_model = glm::rotate(m_model, glm::radians(m_pitch), glm::vec3(0, 1, 0));// Rotation
    m_model = glm::rotate(m_model, glm::radians(m_roll), glm::vec3(0, 0, 1));// Rotation
    m_model = glm::scale(m_model, m_size);     
    ResourceManager::GetShader(m_shader).SetMatrix4("model", m_model);// Scale

    if (m_name != "light")
    {
        ResourceManager::GetShader(m_shader).SetVector3f("material.ambient", m_ambient);
        ResourceManager::GetShader(m_shader).SetVector3f("material.diffuse", m_diffuse);
        ResourceManager::GetShader(m_shader).SetVector3f("material.specular", m_specular);
    }

    if (m_bUseTexture) {
        ResourceManager::GetShader(m_shader).SetVector3f("scaleFactor", m_size);
        ResourceManager::GetShader(m_shader).SetInteger("diffuseTex", 0);
        glActiveTexture(GL_TEXTURE0);
        ResourceManager::GetTexture(m_texture).Bind();
    }

    ResourceManager::GetShader(m_shader).SetFloat("material.shininess", m_shininess);
    ResourceManager::GetShader(m_shader).SetVector3f("color", m_color);
    ResourceManager::GetShader(m_shader).SetInteger("lightMethod", lightMethod);

    ResourceManager::GetShader(m_shader).SetBoolean("bUseTexture", m_bUseTexture);
    ResourceManager::GetShader(m_shader).Use();
    
    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    if (this->m_clicked && this->m_bShowBox) { // bShowBBox
        this->Show2DBBox();
    }
}


// Check object is Clicked

bool Object::CheckClick(double mouseX, double mouseY) const
{
    bool xInRange = (mouseX >= m_bBoxMin.x && mouseX <= m_bBoxMax.x);
    bool yInRange = (mouseY >= m_bBoxMin.y && mouseY <= m_bBoxMax.y);

    return xInRange && yInRange;
}


// Bounding Box

void Object::Get2DBBox(glm::mat4 projection, glm::mat4 view, float screenWidth, float screenHeight)
{
    // 计算立方体的包围框
    glm::vec3 screenMin = glm::vec3(std::numeric_limits<float>::max());
    glm::vec3 screenMax = glm::vec3(std::numeric_limits<float>::lowest());
    m_boxMin = glm::vec2(std::numeric_limits<float>::max());
    m_boxMax = glm::vec2(std::numeric_limits<float>::lowest());
    float depthMin = 1.0f;
    float depthMax = 0.0f;

    //// 顶点数据
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

    // 计算顶点在屏幕空间的位置
    for (int i = 0; i < 8; i++) {
        glm::vec4 clipSpacePos = projection * view * m_model * glm::vec4(vertices[i], 1.0f);
        m_boxMin = glm::min(m_boxMin, glm::vec2(clipSpacePos.x / clipSpacePos.w, clipSpacePos.y/clipSpacePos.w));
        m_boxMax = glm::max(m_boxMax, glm::vec2(clipSpacePos.x / clipSpacePos.w, clipSpacePos.y / clipSpacePos.w));
        glm::vec3 screenSpacePos = glm::vec3((clipSpacePos.x / clipSpacePos.w + 1.0f) * 0.5f * screenWidth,
            (1.0f - clipSpacePos.y / clipSpacePos.w) * 0.5f * screenHeight,
            (clipSpacePos.z / clipSpacePos.w + 1.0f) * 0.5f);
        screenMin = glm::min(screenMin, screenSpacePos);
        screenMax = glm::max(screenMax, screenSpacePos);
        depthMin = glm::min(depthMin, screenSpacePos.z);
        depthMax = glm::max(depthMax, screenSpacePos.z);
    }

    // 存储为二维包围框
    m_bBoxMin = glm::vec2(screenMin.x, screenMin.y);
    m_bBoxMax = glm::vec2(screenMax.x, screenMax.y);

    m_depth = (depthMin + depthMax) * 0.5;
}

void Object::Show2DBBox() {

    GLfloat vertices[] = {
        m_boxMin.x, m_boxMin.y,
        m_boxMax.x, m_boxMin.y,
        m_boxMax.x, m_boxMax.y,
        m_boxMin.x, m_boxMax.y
    };
    GLint indices[] = {  // 注意索引从0开始!
        0, 1, 2,   // 第一个三角形
        2, 3, 0    // 第二个三角形
    };
    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    ResourceManager::GetShader("bbox").Use();

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}
void Object::BindVAO() {
    GLuint cubeVBO = 0;
    GLfloat vertices[] = {
        // Back face
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // Bottom-left
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, // top-right
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,  // top-right
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,  // bottom-left
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,// top-left
        // Front face
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // bottom-right
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,  // top-right
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // top-right
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // top-left
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom-left
        // Left face
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-left
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-left
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // bottom-right
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
        // Right face
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-left
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-right         
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-right
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // top-left
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-left     
        // Bottom face
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, // top-left
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,// bottom-left
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom-left
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom-right
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
        // Top face
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,// top-left
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // top-right     
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,// top-left
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f // bottom-left        
    };
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(m_VAO);
    // Fill buffer
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Link vertex attributes
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glDeleteBuffers(1, &cubeVBO);
}



