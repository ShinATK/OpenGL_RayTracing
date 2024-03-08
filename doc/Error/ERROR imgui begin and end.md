
> 细节问题

在 GUI 类中实现了一个显示帧率的控件：

```cpp
void GUI::ShowFrameRate(const char* name, float dt)
{
	ImGui::Begin(name);
	ImGui::Separator();

	static double frameTimes[1000];
	static int frameTimeIdx = 0;

	frameTimes[frameTimeIdx] = dt;
	frameTimeIdx = (frameTimeIdx + 1) % 1000;

	if (frameTimeIdx == 0) return;

	double averageFrameTime = 0.0;
	for (int i = 0; i < frameTimeIdx; ++i)
	{
		averageFrameTime += frameTimes[i];
	}
	averageFrameTime /= frameTimeIdx;

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / this->io->Framerate, this->io->Framerate);

	// 开始绘制帧率图表
	ImPlot::CreateContext();
	if (ImPlot::BeginPlot("Frame Rate", ImVec2(400, 300))) {
		ImPlot::SetupAxis(ImAxis_X1, "Frame");
		ImPlot::SetupAxis(ImAxis_Y1, "Frame Time");
		// 设置 ImPlot 的 X 轴范围为 0 到 1000，Y 轴范围为 0 到 1.0/平均帧时间
		ImPlot::SetupAxisLimitsConstraints(ImAxis_X1, 0, 1000);
		ImPlot::SetupAxisLimitsConstraints(ImAxis_Y1, 0, 0.05);
		// 绘制折线图
		ImPlot::PlotLine("Frame Rate", frameTimes, 1000);
		ImPlot::EndPlot();
	}

	ImGui::Separator();
	ImGui::End();
}
```

运行后在图表显示曲线达到 1000 后会跳出报错提示，检查源码发现如下：

```cpp
"Mismatched Begin/BeginChild vs End/EndChild calls: did you forget to call End/EndChild?"
```

仔细检查后发现问题在于：`if (frameTimeIdx == 0) return;`

因为达到 1000 后会重新从数组开头填充，从而在这里 return，导致函数开头的 begin 没有匹配的 end，调整代码位置后正常。

```cpp
void GUI::ShowFrameRate(const char* name, float dt)
{
	static double frameTimes[1000];
	static int frameTimeIdx = 0;

	frameTimes[frameTimeIdx] = dt;
	frameTimeIdx = (frameTimeIdx + 1) % 1000;

	if (frameTimeIdx == 0) return;

	double averageFrameTime = 0.0;
	for (int i = 0; i < frameTimeIdx; ++i)
	{
		averageFrameTime += frameTimes[i];
	}
	averageFrameTime /= frameTimeIdx;
// 放这里就好了

	ImGui::Begin(name);
	ImGui::Separator();
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / this->io->Framerate, this->io->Framerate);

	// 开始绘制帧率图表
	ImPlot::CreateContext();
	if (ImPlot::BeginPlot("Frame Rate", ImVec2(400, 300))) {
		ImPlot::SetupAxis(ImAxis_X1, "Frame");
		ImPlot::SetupAxis(ImAxis_Y1, "Frame Time");
		// 设置 ImPlot 的 X 轴范围为 0 到 1000，Y 轴范围为 0 到 1.0/平均帧时间
		ImPlot::SetupAxisLimitsConstraints(ImAxis_X1, 0, 1000);
		ImPlot::SetupAxisLimitsConstraints(ImAxis_Y1, 0, 0.05);
		// 绘制折线图
		ImPlot::PlotLine("Frame Rate", frameTimes, 1000);
		ImPlot::EndPlot();
	}

	ImGui::Separator();
	ImGui::End();
}
```


