#pragma once

#include "../resource_manager/ResourceManager.h"

#include <vector>

class Object
{
private:
    // Object state
    const char* m_name;
    glm::vec3   m_position, m_size, m_color;

    glm::vec4   m_rotation;
    float       m_pitch, m_yaw, m_roll;

    glm::mat4   m_model;

    GLboolean   m_shown; // draw object or not
    GLboolean   m_clicked; // cursor clicked or not
    GLboolean   m_bShowBox; // 是否显示 bounding box
    int lightMethod; // phong or blinn-phong

    // Vertice
    GLuint      m_VAO;
    GLuint      m_count;

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

public:
    // Constructor(s)
    Object(const char* name, glm::vec3 pos = glm::vec3(0.0f), glm::vec3 size = glm::vec3(1.0f), GLboolean bUseTexture = false);
    virtual ~Object();

    // Draw
    virtual void BindVAO();
    virtual void Draw();

    // Update Resources
    void UpdateResources(bool bUseTexture);

    // Check click
    bool CheckClick(double mouseX, double mouseY) const;

    // Bounding Box
    void Show2DBBox();
    void Get2DBBox(glm::mat4 projection, glm::mat4 view, float screenWidth, float screenHeight);

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




    // Set

    void SetClicked(bool bClicked);
    void SetShowBox(bool bShow);
    void SetShown(bool bCanSee);

    void SetShader(const std::string Shader);
    void SetTexture(const std::string Texture);

    void SetPosition(glm::vec3 new_position);
    void SetSize(glm::vec3 new_size);

    void SetYaw(float angle);
    void SetPitch(float angle);
    void SetRoll(float angle);

    // Get
    glm::vec3 GetPosition() const;
    glm::vec3 GetSize() const;
    glm::vec4 GetRotation() const;

    float GetPitch() const;
    float GetYaw() const;
    float GetRoll() const;

    const char* GetName() const;
    bool GetClicked() const;

    float GetDepth() const;

private:
    // Update 
    void UpdateShader(bool bUseTexture);
    void UpdateMaterial(); // 传入自定义材质数据，并调用对应的Shader
    void UpdateTexture();
};



