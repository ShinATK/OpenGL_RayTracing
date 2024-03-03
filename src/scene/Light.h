#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "../resource_manager/ResourceManager.h"

class Light {

private:
	glm::vec3 m_position;
	glm::vec3 m_color;
	bool m_shown;

public:
	Light();

	virtual ~Light();

	virtual void Show();
	virtual void Hide();

	virtual void SetPosition(glm::vec3 position);
	virtual void SetPosition(float x, float y, float z);

	virtual void SetColor(glm::vec3 color);
	virtual void SetColor(float R, float G, float B);

	virtual void RenderCube();
};

