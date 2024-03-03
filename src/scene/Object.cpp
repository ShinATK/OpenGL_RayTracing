#include "Object.h"

// Constructor(s)

Object::Object(const char* name, glm::vec3 pos, glm::vec3 size, glm::vec4 rotation, glm::vec3 color) :
    m_name(name),
    m_position(pos), // Ĭ��λ��Ϊԭ��
    m_size(size), // Ĭ�ϳߴ�Ϊ��λ��С
    m_color(color), // Ĭ����ɫΪ��ɫ
    m_rotation(rotation), // Ĭ����תΪ����ת
    m_shown(true), // Ĭ����ʾ����
    m_clicked(false), // Ĭ��δѡ��
    m_bShowBox(false),
    m_VAO(0), // Ĭ��VAOΪ0
    m_count(0), // Ĭ�϶�������Ϊ0
    m_shader("default"), // Ĭ����ɫ������Ϊ��
    m_texture("default"), // Ĭ����������Ϊ��
    m_material("default"), // Ĭ�ϲ�������Ϊ��
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

void Object::Draw() {

    UpdateResources(true);

    if (this->m_shown) {
        glBindVertexArray(m_VAO);
        glDrawArrays(GL_TRIANGLES, 0, m_count);
        glBindVertexArray(0);
    }

    if (this->m_clicked) { // bShowBBox
        this->ShowBBox();
    }
}


// Update Resources

void Object::UpdateResources(bool bUseTexture)
{
    this->UpdateMaterial();
    this->UpdateShader(bUseTexture);
    this->UpdateTexture();
}


// Check object is Clicked

bool Object::CheckClick(double mouseX, double mouseY) const
{
    //std::cout << "BoxMin = " << m_bBoxMin.x << ", " << m_bBoxMin.y << std::endl;
    //std::cout << "BoxMax = " << m_bBoxMax.x << ", " << m_bBoxMax.y << std::endl;

    //std::cout << "boxMin = " << m_boxMin.x << ", " << m_boxMin.y << std::endl;
    //std::cout << "boxMax = " << m_boxMax.x << ", " << m_boxMax.y << std::endl;

    bool xInRange = (mouseX >= m_bBoxMin.x && mouseX <= m_bBoxMax.x);
    bool yInRange = (mouseY >= m_bBoxMin.y && mouseY <= m_bBoxMax.y);

    return xInRange && yInRange;
}


// Set

void Object::SetShown(bool bCanSee)                  { this->m_shown = bCanSee; }

void Object::SetShader(const std::string Shader)     { this->m_shader = Shader; }
void Object::SetTexture(const std::string Texture)   { this->m_texture = Texture; }
void Object::SetMaterial(const std::string Material) { this->m_material = Material; }

void Object::SetPosition(glm::vec3 new_position)     { this->m_position = new_position; }
void Object::SetRotation(glm::vec4 new_rotation)     { this->m_rotation = new_rotation; }
void Object::SetSize(glm::vec3 new_size)             { this->m_size = new_size; }

void Object::SetClicked(bool bClicked)               { this->m_clicked = bClicked; }
void Object::SetShowBox(bool bShow)                  { this->m_bShowBox = bShow; }


// Get
glm::vec3 Object::GetPosition() const { return this->m_position; }
glm::vec4 Object::GetRotation() const { return this->m_rotation; }
glm::vec3 Object::GetSize() const { return this->m_size; }

const char* Object::GetName() const { return this->m_name; }
bool Object::GetClicked() const { return this->m_clicked; }
float Object::GetDepth() const { return this->m_depth; }


// Update material, shader, texture
void Object::UpdateMaterial() {
    // ���ݴ����Material����Shader
    if (m_shader == "default") {
        ResourceManager::GetShader(m_shader).SetInteger("tex", 0);
        ResourceManager::GetShader(m_shader).SetVector3f("color", m_color);
        return;
    }
    if (ResourceManager::CheckMaterial(m_material)) {
        std::string category = ResourceManager::GetMaterial(m_material).GetCategory();
        if (category == m_shader) {
            ResourceManager::GetMaterial(m_material).UpdateShader(ResourceManager::GetShader(m_shader));
        }
        else {
            std::cerr << "Warning: Material '" << m_material << "''s category" << category << " does not match " << "Shader '" << m_shader << "'!" << std::endl;
            std::cerr << "Use default shader instead!" << std::endl;

            m_shader = "default";
            ResourceManager::GetShader(m_shader).SetInteger("tex", 0);
            ResourceManager::GetShader(m_shader).SetVector3f("color", m_color);
            return;
        }
    }
    else {
        std::cerr << "Warning: Material '" << m_material << "' does not exist!" << std::endl;
        std::cerr << "Use default shader instead!" << std::endl;

        m_shader = "default";
        ResourceManager::GetShader(m_shader).SetInteger("tex", 0);
        ResourceManager::GetShader(m_shader).SetVector3f("color", m_color);
        return;
    }
}
void Object::UpdateShader(bool bUseTexture) {
    glm::mat4 model = glm::mat4(1.0f); // һ�����ų�ʼ��
    // Translate
    model = glm::translate(model, m_position);
    // Rotation
    model = glm::translate(model, glm::vec3(0.5f * m_size.x, 0.5f * m_size.y, 0.0f));
    model = glm::rotate(model, m_rotation.w, glm::vec3(m_rotation));
    model = glm::translate(model, glm::vec3(-0.5f * m_size.x, -0.5f * m_size.y, 0.0f));
    // Scale
    model = glm::scale(model, m_size);
    ResourceManager::GetShader(m_shader).SetMatrix4("model", model);
    ResourceManager::GetShader(m_shader).SetBoolean("bUseTexture", bUseTexture);

    ResourceManager::GetShader(m_shader).Use();
}
void Object::UpdateTexture() {
    glActiveTexture(GL_TEXTURE0);
    ResourceManager::GetTexture(m_texture).Bind();
}


// Bounding Box

void Object::Get2DBBox(glm::mat4 projection, glm::mat4 view, float screenWidth, float screenHeight)
{
    glm::mat4 model = glm::mat4(1.0f); // һ�����ų�ʼ��

    // Translate
    model = glm::translate(model, m_position);
    // Rotation
    model = glm::translate(model, glm::vec3(0.5f * m_size.x, 0.5f * m_size.y, 0.0f));
    model = glm::rotate(model, m_rotation.w, glm::vec3(m_rotation));
    model = glm::translate(model, glm::vec3(-0.5f * m_size.x, -0.5f * m_size.y, 0.0f));
    // Scale
    model = glm::scale(model, m_size);

    // ����������İ�Χ��
    glm::vec3 screenMin = glm::vec3(std::numeric_limits<float>::max());
    glm::vec3 screenMax = glm::vec3(std::numeric_limits<float>::lowest());
    m_boxMin = glm::vec2(std::numeric_limits<float>::max());
    m_boxMax = glm::vec2(std::numeric_limits<float>::lowest());
    float depthMin = 1.0f;
    float depthMax = 0.0f;

    //// ��������
    glm::vec3 vertices[] = {
        glm::vec3(-0.5f, -0.5f,  0.5f),
        glm::vec3(0.5f, -0.5f,  0.5f),
        glm::vec3(0.5f,  0.5f,  0.5f),
        glm::vec3(-0.5f,  0.5f,  0.5f),
        // ����
        glm::vec3(-0.5f, -0.5f, -0.5f),
        glm::vec3(0.5f, -0.5f, -0.5f),
        glm::vec3(0.5f,  0.5f, -0.5f),
        glm::vec3(-0.5f,  0.5f, -0.5f)
    };

    // ���㶥������Ļ�ռ��λ��
    for (int i = 0; i < 8; i++) {
        glm::vec4 clipSpacePos = projection * view * model * glm::vec4(vertices[i], 1.0f);
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

    // �洢Ϊ��ά��Χ��
    m_bBoxMin = glm::vec2(screenMin.x, screenMin.y);
    m_bBoxMax = glm::vec2(screenMax.x, screenMax.y);

    m_depth = (depthMin + depthMax) * 0.5;
}

void Object::ShowBBox() {

    GLfloat vertices[] = {
        m_boxMin.x, m_boxMin.y,
        m_boxMin.x, m_boxMax.y,
        m_boxMax.x, m_boxMin.y,
        m_boxMax.x, m_boxMax.y
    };
    GLint indices[] = {  // ע��������0��ʼ!
        0, 1, 3,   // ��һ��������
        2, 3, 0    // �ڶ���������
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

    m_count = sizeof(vertices) / (8 * sizeof(GLfloat));

    glDeleteBuffers(1, &cubeVBO);
}




