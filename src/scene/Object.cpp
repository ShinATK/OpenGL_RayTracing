#include "Object.h"

// Constructor(s)

Object::Object(const char* name, glm::vec3 pos, glm::vec3 size) :
    m_name(name),
    m_position(pos), // 默认位置为原点
    m_size(size), // 默认尺寸为单位大小
    m_rotation(1,0,0,0), m_yaw(0.0f),m_pitch(0.0f),m_roll(0.0f),
    m_model(1.0f),
    m_color(1.0f), // 默认颜色为白色
    m_shown(true), // 默认显示对象
    m_clicked(false), // 默认未选中
    m_bShowBox(false),
    m_VAO(0), // 默认VAO为0
    m_count(0), // 默认顶点数量为0
    m_shader("default"), // 默认着色器名称为空
    m_texture("default"), // 默认纹理名称为空
    m_material("default"), // 默认材质名称为空
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
    
    m_model = glm::mat4(1.0f);                                                        // 初始化
    m_model = glm::translate(m_model, m_position);                                    // Translate
    m_model = glm::rotate(m_model, glm::radians(m_yaw), glm::vec3(1, 0, 0));// Rotation
    m_model = glm::rotate(m_model, glm::radians(m_pitch), glm::vec3(0, 1, 0));// Rotation
    m_model = glm::rotate(m_model, glm::radians(m_roll), glm::vec3(0, 0, 1));// Rotation
    m_model = glm::scale(m_model, m_size);                                            // Scale

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

void Object::SetClicked(bool bClicked)                  { this->m_clicked  = bClicked; }
void Object::SetShowBox(bool bShow)                     { this->m_bShowBox = bShow; }
void Object::SetShown(bool bCanSee)                     { this->m_shown    = bCanSee; }

void Object::SetShader(const std::string Shader)        { this->m_shader   = Shader; }
void Object::SetTexture(const std::string Texture)      { this->m_texture  = Texture; }
void Object::SetMaterial(const std::string Material)    { this->m_material = Material; }

void Object::SetPosition(glm::vec3 new_position)        { this->m_position = new_position; }
void Object::SetSize(glm::vec3 new_size)                { this->m_size     = new_size; }

void Object::SetYaw(float angle)                        
{ 
    this->m_yaw = angle;
    m_rotation = glm::vec4(1, 0, 0, angle);
}
void Object::SetPitch(float angle)                      
{ 
    this->m_pitch = angle; 
    m_rotation = glm::vec4(0, 1, 0, angle);
}
void Object::SetRoll(float angle)                       
{ 
    this->m_roll = angle; 
    m_rotation = glm::vec4(0, 0, 1, angle);
}


// Get
const char* Object::GetName() const { return this->m_name; }
bool Object::GetClicked() const { return this->m_clicked; }
float Object::GetDepth() const { return this->m_depth; }

glm::vec3 Object::GetPosition() const { return this->m_position; }
glm::vec3 Object::GetSize() const { return this->m_size; }
glm::vec4 Object::GetRotation() const { return this->m_rotation; }

float Object::GetPitch() const { return this->m_pitch; }
float Object::GetYaw() const { return this->m_yaw; }
float Object::GetRoll() const { return this->m_roll; }


// Update material, shader, texture
void Object::UpdateMaterial() {
    // 根据传入的Material设置Shader
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
    ResourceManager::GetShader(m_shader).SetMatrix4("model", m_model);
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
void Object::ShowBBox() {

    GLfloat vertices[] = {
        m_boxMin.x, m_boxMin.y,
        m_boxMin.x, m_boxMax.y,
        m_boxMax.x, m_boxMin.y,
        m_boxMax.x, m_boxMax.y
    };
    GLint indices[] = {  // 注意索引从0开始!
        0, 1, 3,   // 第一个三角形
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

    m_count = sizeof(vertices) / (8 * sizeof(GLfloat));

    glDeleteBuffers(1, &cubeVBO);
}


// Tools function
// vec4: (a, theta)
// quat: (a sin(theta/2), cos(theta/2))
glm::vec4 Object::ToQuat(glm::vec4 vec4)
{
    glm::vec3 a = glm::vec3(vec4);
    float theta = vec4[3];
    return { a * glm::sin(theta / 2), glm::cos(theta / 2) };

}

glm::vec4 Object::QuaternionMultiplication(glm::vec4 q1, glm::vec4 q2) {
    float w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;
    float x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y;
    float y = q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x;
    float z = q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w;
    return glm::vec4(x, y, z, w);
}


