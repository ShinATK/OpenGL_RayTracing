#include "Scene.h"

Scene::Scene(GLFWwindow* window, const GLuint width, const GLuint height) :m_width(width), m_height(height), m_window(window), m_camera(nullptr), m_chosenObject(nullptr), Objects(), m_showSkybox(false)
{
	// Camera
	m_camera = new Camera();
	m_projection = glm::perspective(m_camera->GetZoom(), (float)m_width / (float)m_height, m_camera->GetNear(), m_camera->GetFar());

	// Load Default
	ResourceManager::LoadShader("./resources/shaders/default.vert", "./resources/shaders/default.frag", "default");
	ResourceManager::LoadTexture("./resources/textures/default.png", GL_TRUE, "default");

	// Skybox
	std::vector<std::string> faces
	{
		"./resources/textures/skybox/right.jpg",
		"./resources/textures/skybox/left.jpg",
		"./resources/textures/skybox/bottom.jpg",
		"./resources/textures/skybox/top.jpg",
		"./resources/textures/skybox/front.jpg",
		"./resources/textures/skybox/back.jpg"
	};
	ResourceManager::LoadCubeMap(faces, GL_FALSE, "skybox");
	ResourceManager::LoadShader("./resources/shaders/skybox.vert", "./resources/shaders/skybox.frag", "skybox");
	ResourceManager::GetShader("skybox").SetInteger("skyboxTexture", 0);
}

Scene::~Scene() {
	delete m_camera; m_camera = nullptr;
	m_chosenObject = nullptr;

	ClearAllObject();

	ResourceManager::Clear();

	m_window = nullptr;
	glfwTerminate();
}

void Scene::Init()
{
	// 加载 shader
	ResourceManager::LoadShader("./resources/shaders/bbox.vert", "./resources/shaders/bbox.frag", "bbox");
	ResourceManager::LoadShader("./resources/shaders/basicLight.vert", "./resources/shaders/basicLight.frag", "basicLight");

	// 加载 texture
	ResourceManager::LoadTexture("./resources/textures/container.jpg", GL_FALSE, "container");
	ResourceManager::LoadTexture("./resources/textures/wood.png", GL_TRUE, "wood");

	// 创建 objects
	Objects["container"] = new Object("container", "container", GL_TRUE, glm::vec3(0, 0, -3), glm::vec3(1.0f));
	Objects["floor"] = new Object("floor", "wood", GL_TRUE, glm::vec3(0, -1, 0), glm::vec3(10.0f, 1.0f, 10.0f));

	// 创建 lights
	Lights["light01"] = new Light(glm::vec3(1.2f, 1.0f, 2.0f));
}

void Scene::ProcessInput(GLfloat dt)
{
	// Check camera movement
	m_camera->ProcessInput(m_window, dt);
}

void Scene::Update()
{
	// Update view matrix
	glm::mat4 view    = m_camera->GetViewMatrix();
	glm::vec3 viewPos = m_camera->GetPosition();
	// ResourceManager 自动更新使用中的shader view matrix
	ResourceManager::UpdateShader(m_projection, view, viewPos);
	// 防止 skybox 随着 camera 移动而移动
	ResourceManager::GetShader("skybox").SetMatrix4("view", glm::mat4(glm::mat3(view)));

	// Update Bounding Box
	if (m_chosenObject != nullptr)
		m_chosenObject->Get2DBBox(m_projection, view, m_width, m_height);

	// Update light uniform
	ResourceManager::GetShader("basicLight").SetVector3f("light.position", Lights["light01"]->GetPosition());
	ResourceManager::GetShader("basicLight").SetVector3f("light.ambient", Lights["light01"]->GetAmbient());
	ResourceManager::GetShader("basicLight").SetVector3f("light.diffuse", Lights["light01"]->GetDiffuse());
	ResourceManager::GetShader("basicLight").SetVector3f("light.specular", Lights["light01"]->GetSpecular());
}

void Scene::Render()
{
	// Render
	if (this->m_showSkybox) this->DrawSkybox(ResourceManager::GetShader("skybox"));
	this->DrawObjects();
}

void Scene::DrawObjects()
{
	for (auto& item : Objects)
	{
		if (item.second->GetShown())
			item.second->Draw("basicLight");
	}
	for (auto& item : Lights)
	{
		if (item.second->GetShown())
			item.second->Draw("default");
	}
}


void Scene::ClearAllObject()
{
	for (auto& item : Objects)
	{
		delete item.second; item.second = nullptr;
	}
	Objects.clear();

	for (auto& item : Lights)
	{
		delete item.second; item.second = nullptr;
	}
	Lights.clear();
}


void Scene::ApplyTo(std::string Category, std::string ObjectName, std::string Shader, std::string Texture)
{
	if (Objects.find(ObjectName) != Objects.end())
	{
		Objects[ObjectName]->SetShader(Shader);
		Objects[ObjectName]->SetTexture(Texture);
	}
	else
	{
		std::cerr << "Warning: Object " << ObjectName << " not found!" << std::endl;
		return;
	}
}



void Scene::CheckClicked(double mouseX, double mouseY)
{
	float pre_depth = 1.0f;
	for (auto& item : Objects) {
		item.second->Get2DBBox(m_projection, m_camera->GetViewMatrix(), m_width, m_height);
		if (item.second->CheckClick(mouseX, mouseY) && pre_depth > item.second->GetDepth()) {
			item.second->SetClicked(true);
			this->AddClicked(item.second);
			pre_depth = item.second->GetDepth();
		}
	}
}

void Scene::AddClicked(Object* object)
{
	// 之前未选中
	if (m_chosenObject == nullptr) {
		m_chosenObject = object;
		std::cout << "选中物体" << std::endl;
		return;
	}

	// 之前已选中，但是自身
	if (m_chosenObject == object) {
		std::cout << "选中自身" << std::endl;
		return;
	}

	// 之前已选中，更新为新物体
	m_chosenObject->SetClicked(false);
	m_chosenObject = object;
	return;
}


void Scene::DrawSkybox(Shader& shader)
{
	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};
	
	unsigned int skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glBindVertexArray(0);
	glDeleteBuffers(1, &skyboxVBO);

	glActiveTexture(GL_TEXTURE0);
	ResourceManager::GetTexture("skybox").Bind(true);

	shader.Use();
	glDepthFunc(GL_LEQUAL); // 如果是GL_LESS，会导致位于1.0远平面位置出现闪烁
	glBindVertexArray(skyboxVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS);
}

void Scene::SetCameraView(GLfloat xpos, GLfloat ypos, GLfloat& lastX, GLfloat& lastY, GLboolean bCatchMouse, GLboolean& bFirstMouse)
{
	if (bCatchMouse)
	{
		if (bFirstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			bFirstMouse = GL_FALSE;
		}
		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos;

		lastX = xpos;
		lastY = ypos;

		float sensitivity = 0.5;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		m_camera->ViewMovement(xoffset, yoffset);
	}
	else
	{
		lastX = xpos;
		lastY = ypos;
	}
}

Object* Scene::GetChosenObject()
{
	return m_chosenObject;
}

void Scene::ClearChosenObject()
{
	if (m_chosenObject) {
		m_chosenObject->SetClicked(false);
		m_chosenObject = nullptr;
	}
}

GLboolean Scene::GetShowSkybox() const
{
	return this->m_showSkybox;
}

void Scene::SetShowSkybox(GLboolean show)
{
	this->m_showSkybox = show;
}



template<typename T>
void ClearPtr(std::vector<T*> ptr)
{
	for (auto i : ptr)
	{
		delete i;
		i = nullptr;
	}
}