
> 什么奇奇怪怪的 bug 都能让我碰上

在渲染 cube 和 gui 的时候，发现视图内全黑，最后发现把 `glClearColor()` 位置放错，导致先绘制的 cube 被遮挡

```cpp
inline void Scene::Render()
{
    cube->Draw(*cubeRenderer);
	gui->Render();

	// 错误的位置
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
```

修改后：

```cpp
inline void Scene::Render()
{
	// 正确的
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    cube->Draw(*cubeRenderer);
	gui->Render();
}
```

