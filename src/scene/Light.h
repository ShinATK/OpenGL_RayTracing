#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "../resource_manager/ResourceManager.h"

class Light {

private:
	glm::vec3 m_position;
	glm::vec3 m_size;
	glm::vec3 m_color;
	glm::vec3 m_ambient;
	glm::vec3 m_diffuse;
	glm::vec3 m_specular;

	bool m_shown;

	GLuint m_VAO;

public:
	Light(glm::vec3 position, glm::vec3 color=glm::vec3(1.0f), glm::vec3 ambient = glm::vec3(0.1f), glm::vec3 diffuse = glm::vec3(1.0f), glm::vec3 specular = glm::vec3(0.3f));

	~Light();

	void Draw(std::string new_shader = "default");

	bool GetShown() const { return this->m_shown; }
	void SetShown(bool bCanSee) { this->m_shown = bCanSee; }

	glm::vec3 GetPosition() const { return this->m_position; }
	void SetPosition(glm::vec3 position) { this->m_position = position; }
	void SetPosition(float x, float y, float z) { this->m_position = glm::vec3(x, y, z); }

	glm::vec3 GetColor() const { return this->m_color; }
	void SetColor(glm::vec3 color) { this->m_color = color; }
	void SetColor(float R, float G, float B) { this->m_color = glm::vec3(R, G, B); }

	glm::vec3 GetSize() const { return this->m_size; }
	void SetSize(glm::vec3 size) { this->m_size = size; }
	void SetSize(float x, float y, float z) { this->m_size = glm::vec3(x, y, z); }

	glm::vec3 GetAmbient() const { return this->m_ambient; }
	glm::vec3 GetDiffuse() const { return this->m_diffuse; }
	glm::vec3 GetSpecular() const { return this->m_specular; }

private:
	
	void BindVAO();
};

