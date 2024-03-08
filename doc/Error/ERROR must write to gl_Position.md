
> 什么奇奇怪怪的 bug 都能让我碰上

报错：error C5145: must write to gl_Position

经检查，发现是因为 vs 的相对路径识别问题

> vs 中相对路径以**项目文件**所在目录进行解析

出问题代码：

```cpp
// vs 中错误的相对路径
ResourceManager::LoadShader("../resources/shaders/object.vert", "../resources/shaders/object.frag", "cubeShader");
ResourceManager::LoadTexture("../resources/textures/container.jpg", GL_FALSE, "cubeTex");
```

如上方错误代码所示，相对路径是参照当前文件的方式书写的，而 vs 应当是相对于项目文件，正确如下：

```cpp
// vs 中正确的相对路径
ResourceManager::LoadShader("./resources/shaders/object.vert", "./resources/shaders/object.frag", "cubeShader");
ResourceManager::LoadTexture("./resources/textures/container.jpg", GL_FALSE, "cubeTex");
```

修改后问题解决

