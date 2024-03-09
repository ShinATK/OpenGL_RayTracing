#pragma once

#include "../components/Camera.h"
#include "../components/Renderer.h"
#include "../components/GUI.h"
#include "../resource_manager/ResourceManager.h"

#include "Object.h"
#include "Light.h"

#include <unordered_map>

template<typename T>
void ClearPtr(std::vector<T*> ptr);

class Scene
{
private:
	GLFWwindow*			m_window;
	GLuint				m_width;
	GLuint				m_height;

	Camera*				m_camera;
	Renderer* m_DepthMap;
	glm::mat4			m_projection;

	Object*				m_chosenObject; // chosen object

	GLboolean			m_showSkybox;

	// TODO: 将light作为object的派生类实现
	std::unordered_map<std::string, Object*> Objects;
	std::unordered_map<std::string, Light*> Lights;

public:
	Scene(GLFWwindow* window, const GLuint width, const GLuint height);
	~Scene();

	void Init();

	void ProcessInput(GLfloat dt);
	void Update();
	void Render();

	// Add/remove objects into scene
	void ClearAllObject();

	// Configure
	void ApplyTo(std::string Category, std::string ObjectName, std::string Shader = "default", std::string Texture = "default");

	// Draw
	void DrawObjects();
	void DrawLights();


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


