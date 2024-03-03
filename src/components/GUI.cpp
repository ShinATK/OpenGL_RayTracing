#include "GUI.h"

#include <string>

GUI::GUI(GLFWwindow* window) : io(nullptr), clearColor(0.0f, 0.0f, 0.0f, 1.00f)
{
	// 设置 imgui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	io = &ImGui::GetIO();
	io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// 设置 imgui 外观
	ImGui::StyleColorsDark();
	// 设置渲染后台
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();
}

GUI::~GUI()
{
	ImPlot::DestroyContext();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void GUI::Init(bool bCatch)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	this->CatchMouse(bCatch);
}

void GUI::MainWidget(const char* object_name)
{
	ImGui::Begin(object_name);
	{
		this->ShowFrameRate();
		this->SetClearColor();
	}
	ImGui::End();
}

void GUI::Render()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUI::SetBool(const char* usefor, bool& value, bool bSameLineWithFront)
{
	if (bSameLineWithFront) ImGui::SameLine();
	bool temp = value;
	ImGui::Checkbox(usefor, &temp);
	value = temp;
}

void GUI::SetVec3(const char* usefor, float* vector3f, int* limits)
{
	ImGui::Text(usefor);
	ImGui::SliderFloat((std::string(usefor) + std::string(".x")).c_str(), vector3f, limits[0], limits[1]);
	ImGui::SliderFloat((std::string(usefor) + std::string(".y")).c_str(), vector3f + 1, limits[0], limits[1]);
	ImGui::SliderFloat((std::string(usefor) + std::string(".z")).c_str(), vector3f + 2, limits[0], limits[1]);
	ImGui::Separator();
}


void GUI::SetFloat(const char* usefor, float& value, int* limits)
{
	float temp = value;
	ImGui::SliderFloat(usefor, &temp, limits[0], limits[1]);
	ImGui::Separator();
	value = temp;
}


/* Private */

void GUI::CatchMouse(bool bCatch)
{
	if (bCatch)
		io->ConfigFlags |= ImGuiConfigFlags_NoMouse;
	else
		io->ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
}

void GUI::SetClearColor()
{
	ImGui::ColorEdit3("Clear Color", (float*)&(this->clearColor)); ImGui::Separator();
}
void GUI::ShowFrameRate()
{
	static double frameTimes[1000];
	static int frameTimeIdx = 0;

	frameTimes[frameTimeIdx] = 1000.0f / this->io->Framerate;
	frameTimeIdx = (frameTimeIdx + 1) % 1000;

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / this->io->Framerate, this->io->Framerate);
	// 开始绘制帧率图表
	ImPlot::CreateContext();
	if (ImPlot::BeginPlot("Frame Rate", ImVec2(400, 300))) {
		ImPlot::SetupAxis(ImAxis_X1, "Frame");
		ImPlot::SetupAxis(ImAxis_Y1, "Frame Time(ms)");
		ImPlot::SetupAxisLimitsConstraints(ImAxis_X1, 0, 1000);// 设置ImPlot的X轴范围为0到1000
		ImPlot::PlotLine("Frame Rate", frameTimes, 1000);      // 绘制折线图
		ImPlot::EndPlot();
	} ImGui::Separator();
}
