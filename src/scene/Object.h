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

    // Vertice
    GLuint      m_VAO;
    GLuint      m_count;

    // Shader name
    std::string m_shader;
    // Texture name
    std::string m_texture;
    // Material name
    std::string m_material;

    // Bounding Box
    float       m_depth;
    glm::vec2   m_bBoxMin;
    glm::vec2   m_bBoxMax;
    glm::vec2   m_boxMin;
    glm::vec2   m_boxMax;

public:
    // Constructor(s)
    Object(const char* name, glm::vec3 pos, glm::vec3 size=glm::vec3(1.0f));
    virtual ~Object();

    // Draw
    virtual void BindVAO();
    virtual void Draw();

    // Update Resources
    void UpdateResources(bool bUseTexture);

    // Check click
    bool CheckClick(double mouseX, double mouseY) const;

    // Bounding Box
    void ShowBBox();
    void Get2DBBox(glm::mat4 projection, glm::mat4 view, float screenWidth, float screenHeight);


    // Input
    void RotateX(float angle);
    void RotateY(float angle);
    void RotateZ(float angle);


    // Set

    void SetClicked(bool bClicked);
    void SetShowBox(bool bShow);
    void SetShown(bool bCanSee);

    void SetShader(const std::string Shader);
    void SetTexture(const std::string Texture);
    void SetMaterial(const std::string Material);

    void SetPosition(glm::vec3 new_position);
    void SetSize(glm::vec3 new_size);
    void SetRotation(float angle, int axis);

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

    glm::vec4 ToQuat(glm::vec4 vec4);
    glm::vec4 ToVec4(glm::vec4 quat);
    glm::vec4 QuaternionMultiplication(glm::vec4 q1, glm::vec4 q2);
};



