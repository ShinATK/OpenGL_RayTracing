
使用 OpenGL 中的 `GLboolean` 类型变量进行布尔类型取反操作，结果 vs 总是跳出使用 `~` 来按位取反的提示

`GLboolean` 的定义为宏：

```cpp
#define GL_TRUE  1;
#define GL_FALSE 0;
```

所以实际上是 `unsigned char`


和 `bool` 的区别如下：

1. **类型**：二者都表示布尔值，但`GLboolean` 实际上是一个 `unsigned char`
2. **取值**：`GLboolean` 取值范围是 0 和 1；`bool` 取值为 `true` 或 `false`
3. **隐式转换**：`GLboolean` 不会自动转换为 `bool`，需要显式转换。`bool` 则可以隐式转为 `GLboolean`

```cpp
GLboolean glBool = GL_TRUE;
if (static_cast<bool>(glBool)) {
    // 执行代码
}
```

