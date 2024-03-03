
> 细节！细节！细节！

之前学习 OpenGL 的时候使用 ImGui 遇到过一次控件命名冲突的问题，这次虽然能知道是命名冲突，但却忽略了最明显的位置的错误。

出错的代码如下：

```cpp
// Set position
static float pos[3];
int pos_limits[2] = {-10, 10};
ToFloat3<glm::vec3, float>(scene->GetChosenObject()->GetPosition(), pos);
gui->SetVec3("SetPosition", pos, pos_limits);
scene->GetChosenObject()->SetPosition(ToVec3<float, glm::vec3>(pos));

// Set Size
static float size[3];
int size_limits[2] = {0, 5};
ToFloat3<glm::vec3, float>(scene->GetChosenObject()->GetSize(), size);
gui->SetVec3("SetSize", size, size_limits);
scene->GetChosenObject()->SetSize(ToVec3<float, glm::vec3>(size));
```

这里本来是用于控制物体的位置和大小的，结果运行后滑动一个控件，另一个控件也会滑动。

主函数这里的控件命名没有冲突，问题出在`gui->SetVec3();`内，函数定义如下：

```cpp
void GUI::SetVec3(const char* usefor, float* vector3f, int* limits)
{
	ImGui::Text(usefor);
	ImGui::SliderFloat("vec.x", vector3f, limits[0], limits[1]);
	ImGui::SliderFloat("vec.y", vector3f + 1, limits[0], limits[1]);
	ImGui::SliderFloat("vec.z", vector3f + 2, limits[0], limits[1]);
	ImGui::Separator();
}
```

问题出在哪里呢？**在这里`vec.x, vec.y, vec.z`**

由于封装成了函数，传入了 usefor 本来就是为了设置控件的名称标识的，结果忘了最重要的部分。

修改如下后运行无误

```cpp
void GUI::SetVec3(const char* usefor, float* vector3f, int* limits)
{
	ImGui::Text(usefor);
	ImGui::SliderFloat((std::string(usefor) + std::string(".x")).c_str(), vector3f, limits[0], limits[1]);
	ImGui::SliderFloat((std::string(usefor) + std::string(".y")).c_str(), vector3f + 1, limits[0], limits[1]);
	ImGui::SliderFloat((std::string(usefor) + std::string(".z")).c_str(), vector3f + 2, limits[0], limits[1]);
	ImGui::Separator();
}
```


