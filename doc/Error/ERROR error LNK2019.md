

这次遇到的 `LNK2019` 问题是使用静态成员模板函数时


我在 `resource_manager.h` 中定义了静态成员模板函数

```cpp
template<typename T>
static std::shared_ptr<T> getResource(const std::string& name);
```

而具体实现放在了 `resource_manager.cpp` 中，当我在其他文件中调用这个函数时 `ResourceManager::getResource(name)` 出现如下错误（错误不是针对该函数的，这里就是举个例子）

```
error LNK2019: 无法解析的外部符号 "public: static void __cdecl ResourceManager::loadResource<class Shader>(class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> > const &)" (??$loadResource@VShader@@@ResourceManager@@SAXAEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z)，函数 "public: virtual void __cdecl Object::setShaderName(class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> > const &)" (?setShaderName@Object@@UEAAXAEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z) 中引用了该符号
```

原因是在编译阶段模板函数就需要生成对应的实例，而此时编译器只能找到 .h 中的声明而找不到 .cpp 中的定义从而导致链接错误。


参考：[C++ Templates Tutorial](https://users.cis.fiu.edu/~weiss/Deltoid/vcstl/templates)的 Implementing class template member functions 部分


也可以通过在声明和定义之间加上 inline 关键字从而让编译器将其视为内联函数来避免链接错误

