#pragma once

#include "../components/Camera.h"

#include "../resource_manager/ResourceManager.h"

#include "Object.h"

#include <unordered_map>

template<typename T>
void ClearPtr(std::vector<T*> ptr);

struct Light {
	glm::vec3 m_position;
	glm::vec3 m_ambient;
	glm::vec3 m_diffuse;
	glm::vec3 m_specular;
	Light(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) :
		m_position(position), m_ambient(ambient), m_diffuse(diffuse), m_specular(specular)
	{}
};

class Scene
{
private:
	GLFWwindow*			m_window;
	GLuint				m_width;
	GLuint				m_height;

	Camera*				m_camera;
	glm::mat4			m_projection;

	Object*				m_chosenObject; // chosen object

	GLboolean			m_showSkybox;

	//TODO：增加检测，设置物体上限
	std::unordered_map<std::string, std::unordered_map<std::string, Object*>> Objects;

	// TODO: 将light作为object的派生类实现
	std::vector<Light*> Lights;

public:
	Scene(GLFWwindow* window, const GLuint width, const GLuint height);
	~Scene();

	void Init();

	void ProcessInput(GLfloat dt);
	void Update();
	void Render(GLfloat dt);

	// Add/remove objects into scene
	void AddObject(Object* object);
	void ClearObject(std::string category, std::string name);
	void ClearObjects();

	// Configure
	void ApplyTo(std::string Category, std::string ObjectName, std::string Shader = "default", std::string Texture = "default");

	// Draw
	void DrawObjects();


	// Add clicked object
	void CheckClicked(double mouseX, double mouseY);
	void AddClicked(Object* object);
	
	// Callback
	void SetCameraView(GLfloat xpos, GLfloat ypos, GLfloat& lastX, GLfloat& lastY, GLboolean bCatchMouse, GLboolean& bFirstMouse);

	void ClearChosenObject();

	// Get


	Object* GetChosenObject();

	GLboolean GetShowSkybox() const;
	void SetShowSkybox(GLboolean show);

	Camera* GetCamera() const { return this->m_camera; }

private:

	void DrawSkybox(Shader& shader);
};


