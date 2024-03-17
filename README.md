# OpenGL Renderer Box

基于 OpenGL 的简陋 Demo

目的是把学到的东西用起来，在用的过程中思考如何解决问题。

顺带多用用 C++，多踩踩坑。


# 进度记录

- [x] 鼠标选择、物体的屏幕空间包围框
- [x] 物体位置、大小、旋转控件

| Position | Size | Rotate |
|----------|------|--------|
| ![](img/GUI_Control_Position.gif) | ![](img/GUI_Control_Rotation.gif)  | ![](img/GUI_Control_Size.gif) |


- [x] 缩放不影响贴图

![](img/TextureMapping_ConsistentScaling.gif)


- [x] Phong、Blinn-Phong 光照模型

| Phong | Blinn-Phong |
|----------|------|
| ![](img/Phong.png) | ![](img/Blinn-Phong.png)  |


反光度为 0.5 时，Phong 和 Blinn-Phong 着色效果对比：

| Phong (shininess = 0.5) | Blinn-Phong (shininess = 0.5) |
|----------|------|
| ![](img/Phong_Shininess_0.5.png) | ![](img/Blinn-Phong_Shininess_0.5.png)  |


- [x] [阴影](./doc/Shadow.md)


![](img/DepthMapping_ParallelLightShadow.gif)


- [x] IBL

![](img/IBL.gif)


- [x] BVH

![](img/bvh_bounding_box.gif)
