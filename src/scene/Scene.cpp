#include "Scene.h"

Scene::Scene(GLFWwindow* window, const GLuint width, const GLuint height) :m_width(width), m_height(height), m_window(window), m_camera(nullptr), m_chosenObject(nullptr), Objects(), m_showSkybox(false)
{
	// Camera
	m_camera = new Camera();
	m_projection = glm::perspective(m_camera->zoom_, (float)m_width / (float)m_height, m_camera->near_, m_camera->far_);

	// Player
	m_player = new Object("player0", glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.2f));

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
	ClearObjects();

	for (auto& light : Lights) {
		delete light; light = nullptr;
	}

	ResourceManager::Clear();

	m_window = nullptr;
	glfwTerminate();
}

void Scene::Init()
{
	// 加载 shader
	ResourceManager::LoadShader("./resources/shaders/bbox.vert", "./resources/shaders/bbox.frag", "bbox");
	ResourceManager::LoadShader("./resources/shaders/phong.vert", "./resources/shaders/phong.frag", "phong");

	// 加载 texture
	ResourceManager::LoadTexture("./resources/textures/container.jpg", GL_FALSE, "container");
	ResourceManager::LoadTexture("./resources/textures/wood.png", GL_TRUE, "wood");
	
	// 加载 material
	ResourceManager::LoadMaterial("phongMaterial", "phong", "./resources/materials/default.txt");

	// 创建 objects
	Objects["Object"]["container"]   = new Object("container", glm::vec3(0, 0, -3));
	Objects["Object"]["floor"]  = new Object("floor", glm::vec3(0, -1, 0));

	// 创建 lights
	Objects["Light"]["light"]   = new Object("light", glm::vec3(1.2f, 1.0f, 2.0f), glm::vec3(0.5f));
	Lights.push_back(new Light(Objects["Light"]["light"]->GetPosition(), glm::vec3(0.2f), glm::vec3(0.5f), glm::vec3(1.0f)));
}

void Scene::Render(GLfloat dt)
{

	// Render
	if(this->m_showSkybox) this->DrawSkybox(ResourceManager::GetShader("skybox"));
	this->DrawObjects();
	//this->m_player->Draw();

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
	glm::vec3 viewPos = m_camera->position_;
	// ResourceManager 自动更新使用中的shader view matrix
	ResourceManager::UpdateShader(m_projection, view, viewPos);
	// 防止 skybox 随着 camera 移动而移动
	ResourceManager::GetShader("skybox").SetMatrix4("view", glm::mat4(glm::mat3(view)));

	// Update position
	
	// 更新 light 位置（light 跟随可视化光源位置的 cube 移动）
	Lights[0]->m_position = Objects["Light"]["light"]->GetPosition();

	// 更新 player 位置
	m_player->SetPosition(m_camera->position_ + glm::normalize(m_camera->front_));
	
	// TODO：player 随视角转动，保证正面朝前
	// TODO: 视角旋转时，camera应当在以player所在位置为中心，以player到camera距离为半径的球面上旋转
	// Objects["Object"]["player"]->SetRotation();
	// m_camera->GetRotation();



	// Update Bounding Box
	if (m_chosenObject != nullptr)
		m_chosenObject->Get2DBBox(m_projection, view, m_width, m_height);

	// Update light uniform
	ResourceManager::GetShader("phong").SetVector3f("light.position",	Lights[0]->m_position);
	ResourceManager::GetShader("phong").SetVector3f("light.ambient",	Lights[0]->m_ambient);
	ResourceManager::GetShader("phong").SetVector3f("light.diffuse",	Lights[0]->m_diffuse);
	ResourceManager::GetShader("phong").SetVector3f("light.specular",	Lights[0]->m_specular);
	
	// TODO: 设置 Enum 来规范 Category，Shader， Material 的名称
	// TODO: 自动遍历并应用
	// TODO：增加 Object 的 shader、texture、material 在 Init 中的初始化
	// TODO：增加 shader、texture、material 在 Update 中的更改
	/*		Category	Name			Shader			Texture			Material */
	//ApplyTo("Player",	"player",		"default",		"default",			"default");
	ApplyTo("Object",	"container",	"phong",		"container",	"phongMaterial");
	ApplyTo("Object",	"floor",		"phong",		"wood",			"phongMaterial");
	ApplyTo("Light",	"light",		"default",		"default",		"default");
}

void Scene::ClearObject(std::string category, std::string name)
{
	auto it = Objects[category].find(name);
	if (it != Objects[category].end())
	{
		delete it->second;
		Objects[category].erase(it);
	}
	else
	{
		std::cerr << "Warning: Object '" << name << "' not found!" << std::endl;
		std::cerr << "Delete Failed!" << std::endl;
	}
	
}

void Scene::ClearObjects()
{
	for (auto& category : Objects)
	{
		for (auto& item : category.second)
		{
			delete item.second; item.second = nullptr;
		}
		category.second.clear();
	}
	Objects.clear();
}


void Scene::ApplyTo(std::string Category, std::string ObjectName, std::string Shader, std::string Texture, std::string Material)
{
	if (Category == "Player") {
		if (m_player == nullptr) {
			std::cerr << "Warning: Player not found!" << std::endl;
			return;
		}
		m_player->SetShader(Shader);
		m_player->SetTexture(Texture);
		m_player->SetMaterial(Material);
		return;
	}
	if (Objects[Category].find(ObjectName) != Objects[Category].end())
	{
		Objects[Category][ObjectName]->SetShader(Shader);
		Objects[Category][ObjectName]->SetTexture(Texture);
		Objects[Category][ObjectName]->SetMaterial(Material);
	}
	else
	{
		std::cerr << "Warning: Object " << ObjectName << " not found!" << std::endl;
		return;
	}
}

void Scene::DrawObjects()
{
	// TODO: 遍历Objects，自动绘制shown=true的object，将应用shader的步骤放在Update中
	for (auto& category : Objects)
	{
		for (auto& item : category.second)
		{
			item.second->Draw();
		}
	}
}

void Scene::CheckClicked(double mouseX, double mouseY)
{
	float pre_depth = 1.0f;
	bool checkAll = false; // 检查本次点击中是否有选中的
	// TODO：增加功能，遍历完后只把深度最小的 AddClicked
	for (auto& category : Objects) {
		if (category.first == "player") continue;
		for (auto& item : category.second) {
			item.second->Get2DBBox(m_projection, m_camera->GetViewMatrix(), m_width, m_height);
			if (item.second->CheckClick(mouseX, mouseY) && pre_depth > item.second->GetDepth()) {
				// TODO: CheckClick 里会进行是否点击命中的设置，导致目前出现的bug。
				item.second->SetClicked(true);
				this->AddClicked(item.second);
				pre_depth = item.second->GetDepth();
				checkAll = true;
			}
		}
	}

// 现在修改为了右键取消选择
	// 都没选中，说明点击的是空白位置，清空 m_chosenObject
	//if (!checkAll && m_chosenObject) {
	//	m_chosenObject->SetClicked(false);
	//	m_chosenObject = nullptr;
	//}
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