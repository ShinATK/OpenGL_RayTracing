#pragma once

#include "../resource_manager/ResourceManager.h"

class Object
{
private:
    // Object state
    const char* m_name;
    glm::vec3   m_position, m_size, m_color;

    float       m_pitch, m_yaw, m_roll;

    glm::mat4   m_model;

    GLboolean   m_shown; // draw object or not
    GLboolean   m_clicked; // cursor clicked or not
    GLboolean   m_bShowBox; // ÊÇ·ñÏÔÊ¾ bounding box
    int lightMethod; // phong or blinn-phong

    // Vertice
    GLuint      m_VAO;

    // Shader name
    std::string m_shader;
    // Texture name
    GLboolean   m_bUseTexture;
    std::string m_texture;

    // Material
    glm::vec3 m_ambient;
    glm::vec3 m_diffuse;
    glm::vec3 m_specular;
    float m_shininess;

    // Bounding Box
    float       m_depth;
    glm::vec2   m_bBoxMin;
    glm::vec2   m_bBoxMax;
    glm::vec2   m_boxMin;
    glm::vec2   m_boxMax;

    // Axis

public:
    // Constructor(s)
    Object(const char* name, std::string texture, GLboolean bUseTexture = GL_TRUE, glm::vec3 pos = glm::vec3(0.0f), glm::vec3 size = glm::vec3(1.0f));
    virtual ~Object();

    // Draw
    virtual void BindVAO();
    virtual void Draw(std::string new_shader);

    // Bounding Box
    void Show2DBBox();
    void Get2DBBox(glm::mat4 projection, glm::mat4 view, float screenWidth, float screenHeight);

    // Local axis
    void CalclulateLocalAxis();
    void ShowAxis(glm::vec3 x, glm::vec3 y, glm::vec3 z);

    // Check click
    bool CheckClick(double mouseX, double mouseY) const;



    // Set & Get
    bool GetShow2DBBox() const { return this->m_bShowBox; }
    void SetShow2DBox(bool bShow) { this->m_bShowBox = bShow; }

    bool GetShown() const { return this->m_shown; }
    void SetShown(bool bCanSee) { this->m_shown = bCanSee; }

    int GetLightMethod() const { return this->lightMethod; }
    void SetLightMethod(int light_model) { this->lightMethod = light_model; }

    void SetUseTexture(GLboolean use_texture) { this->m_bUseTexture = use_texture; }
    GLboolean GetUseTexture() const { return this->m_bUseTexture; }

    void SetAmbient(glm::vec3 new_ambient) { m_ambient = new_ambient; }
    glm::vec3 GetAmbient() const { return m_ambient; }

    void SetDiffuse(glm::vec3 new_diffuse) { m_diffuse = new_diffuse; }
    glm::vec3 GetDiffuse() const { return m_diffuse; }

    void SetSpecular(glm::vec3 new_specular) { m_specular = new_specular; }
    glm::vec3 GetSpecular() const { return m_specular; }

    void SetShininess(float new_shininess) { m_shininess = new_shininess; }
    float GetShininess() const { return m_shininess; }

    void SetClicked(bool bClicked) { this->m_clicked = bClicked; }
    bool GetClicked() const { return this->m_clicked; }

    float GetDepth() const { return this->m_depth; }
    const char* GetName() const { return this->m_name; }

    void SetShader(const std::string Shader) { this->m_shader = Shader; }
    void SetTexture(const std::string Texture) { this->m_texture = Texture; }

    glm::vec3 GetPosition() const { return this->m_position; }
    glm::vec3 GetSize() const { return this->m_size; }
    glm::vec3 GetColor() const { return this->m_color; }
    void SetPosition(glm::vec3 new_position) { this->m_position = new_position; }
    void SetSize(glm::vec3 new_size) { this->m_size = new_size; }
    void SetColor(glm::vec3 new_color) { this->m_color = new_color; }

    float GetYaw() const { return this->m_yaw; }
    float GetPitch() const { return this->m_pitch; }
    float GetRoll() const { return this->m_roll; }
    void SetYaw(float angle) { this->m_yaw = angle; }
    void SetPitch(float angle) { this->m_pitch = angle; }
    void SetRoll(float angle) { this->m_roll = angle; }
};



