#pragma once

#include <glad/glad.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <imgui/implot.h>
#include <imgui/implot_internal.h>

class GUI
{
public:
	ImGuiIO* io;
	ImVec4 clearColor;

	GUI(GLFWwindow* window);
	~GUI();

	void Init(bool bCatch);

	void MainWidget(const char* object_name, bool END=true);

	void Render();

	int SetRadioButton(const char* usefor, int* target, int limit, bool bSameLineWithBack = false);
	bool SetCheckBox(const char* usefor, bool* value, bool bSameLineWithBack = false);

	void SetFloat(const char* usefor, float& value, float v_min, float v_max);
	void SetVec3(const char* usefor, float* vector3f, int* limits);
	
private:

	void SetClearColor();
	void ShowFrameRate();

	void CatchMouse(bool bCatch);
};

