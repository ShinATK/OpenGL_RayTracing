
> 学艺不精，还需努力！

**error LNK2005 错误表示在链接阶段发现了重复定义的符号**

## STB 图像加载库的重复定义问题

问题：`1>Scene.obj : error LNK2005: stbi_convert_iphone_png_to_rgb already defined in RayTracing.obj`

说明，stbi_convert_iphone_png_to_rgb 在 Scene.obj 和 RayTracing.obj 中都被定义了，导致链接时发生冲突。

**STB 图像加载库（STB Image），通常是包含在 .cpp 文件中并且在需要的时候被编译器包含到项目中的**

```cpp
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
```

所以这里的原因可能是：编译器将包含 STB Image 头文件的源文件其编译进了多个目标文件中，从而导致重复定义问题。



## ResourceManager.h 单例模式的实例化

在头文件中定义静态变量，导致其他包含该头文件的源文件都尝试定义这些静态变量，从而导致重复定义的错误。

解决办法：

在头文件中声明

```cpp
// ResourceManager.h

#pragma once

#include <map>
#include <string>

#include "Texture2D.h"
#include "Shader.h"

class ResourceManager
{
public:
    static std::map<std::string, Texture2D> Textures;
    static std::map<std::string, Shader> Shaders;
};
```

将静态变量的定义放在源文件中

```cpp
// ResourceManager.cpp

#include "ResourceManager.h"

std::map<std::string, Texture2D>    ResourceManager::Textures;
std::map<std::string, Shader>       ResourceManager::Shaders;
```




