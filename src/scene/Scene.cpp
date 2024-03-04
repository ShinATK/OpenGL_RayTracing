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
	// ���� shader
	ResourceManager::LoadShader("./resources/shaders/bbox.vert", "./resources/shaders/bbox.frag", "bbox");
	ResourceManager::LoadShader("./resources/shaders/phong.vert", "./resources/shaders/phong.frag", "phong");

	// ���� texture
	ResourceManager::LoadTexture("./resources/textures/container.jpg", GL_FALSE, "container");
	ResourceManager::LoadTexture("./resources/textures/wood.png", GL_TRUE, "wood");
	
	// ���� material
	ResourceManager::LoadMaterial("phongMaterial", "phong", "./resources/materials/default.txt");

	// ���� objects
	Objects["Object"]["container"]   = new Object("container", glm::vec3(0, 0, -3));
	Objects["Object"]["floor"]  = new Object("floor", glm::vec3(0, -1, 0));

	// ���� lights
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
	// ResourceManager �Զ�����ʹ���е�shader view matrix
	ResourceManager::UpdateShader(m_projection, view, viewPos);
	// ��ֹ skybox ���� camera �ƶ����ƶ�
	ResourceManager::GetShader("skybox").SetMatrix4("view", glm::mat4(glm::mat3(view)));

	// Update position
	
	// ���� light λ�ã�light ������ӻ���Դλ�õ� cube �ƶ���
	Lights[0]->m_position = Objects["Light"]["light"]->GetPosition();

	// ���� player λ��
	m_player->SetPosition(m_camera->position_ + glm::normalize(m_camera->front_));
	
	// TODO��player ���ӽ�ת������֤���泯ǰ
	// TODO: �ӽ���תʱ��cameraӦ������player����λ��Ϊ���ģ���player��camera����Ϊ�뾶����������ת
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
	
	// TODO: ���� Enum ���淶 Category��Shader�� Material ������
	// TODO: �Զ�������Ӧ��
	// TODO������ Object �� shader��texture��material �� Init �еĳ�ʼ��
	// TODO������ shader��texture��material �� Update �еĸ���
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
	// TODO: ����Objects���Զ�����shown=true��object����Ӧ��shader�Ĳ������Update��
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
	bool checkAll = false; // ��鱾�ε�����Ƿ���ѡ�е�
	// TODO�����ӹ��ܣ��������ֻ�������С�� AddClicked
	for (auto& category : Objects) {
		if (category.first == "player") continue;
		for (auto& item : category.second) {
			item.second->Get2DBBox(m_projection, m_camera->GetViewMatrix(), m_width, m_height);
			if (item.second->CheckClick(mouseX, mouseY) && pre_depth > item.second->GetDepth()) {
				// TODO: CheckClick �������Ƿ������е����ã�����Ŀǰ���ֵ�bug��
				item.second->SetClicked(true);
				this->AddClicked(item.second);
				pre_depth = item.second->GetDepth();
				checkAll = true;
			}
		}
	}

// �����޸�Ϊ���Ҽ�ȡ��ѡ��
	// ��ûѡ�У�˵��������ǿհ�λ�ã���� m_chosenObject
	//if (!checkAll && m_chosenObject) {
	//	m_chosenObject->SetClicked(false);
	//	m_chosenObject = nullptr;
	//}
}

void Scene::AddClicked(Object* object)
{
	// ֮ǰδѡ��
	if (m_chosenObject == nullptr) {
		m_chosenObject = object;
		std::cout << "ѡ������" << std::endl;
		return;
	}

	// ֮ǰ��ѡ�У���������
	if (m_chosenObject == object) {
		std::cout << "ѡ������" << std::endl;
		return;
	}

	// ֮ǰ��ѡ�У�����Ϊ������
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
	glDepthFunc(GL_LEQUAL); // �����GL_LESS���ᵼ��λ��1.0Զƽ��λ�ó�����˸
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