
> 吃一堑，长一智。希望自己能记住

报错信息如下：

```cpp
| ERROR::SHADER: Compile-time error: Type: FRAGMENT
0(37) : error C7011: implicit cast from "vec4" to "vec3"
0(38) : error C7011: implicit cast from "vec4" to "vec3"
```

经检查，发现在片段着色器中将 `texture()` 返回值直接赋值给了 vec3 类型的变量，如下：

```cpp
vec3 diffuseColor = texture(diffuseTex, TexCoord);
```

**在 OpenGL 着色器语言中，不允许 vec4 到 vec3 的隐式转换**

所以这里应当改为如下：

```cpp
vec3 diffuseColor = texture(diffuseTex, TexCoord).rgb;
```


