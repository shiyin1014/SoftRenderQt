## SoftRenderQt
<img src="https://github.com/shiyin1014/SoftRenderQt/blob/main/image/phong.png" height="50%" width="50%" alt="" /><img src="https://github.com/shiyin1014/SoftRenderQt/blob/main/image/normal.png" height="50%" width="50%" alt="" />

<img src="https://github.com/shiyin1014/SoftRenderQt/blob/main/image/wire_frame.png" height="50%" width="50%" alt="" /><img src="https://github.com/shiyin1014/SoftRenderQt/blob/main/image/depth.png" height="50%" width="50%" alt="" />

本项目是一个基于c++编写的软光栅化渲染器，除了使用opencv和qt进行界面展示和交互外无其他任何依赖，整体代码量约为1700行左右。

### 主要特性
* Blinn-Phong reflection model
* Shader-based
* Perspective correct interpolation
* Back-face culling
* Homogeneous clipping
* Tangent space normal mapping
* Wire frame
* Depth testing
* Orbital camera

### 截图展示
<img src="https://github.com/shiyin1014/SoftRenderQt/blob/main/image/elfgirl.png" width = "50%" height="50%" alt=""/><img src="https://github.com/shiyin1014/SoftRenderQt/blob/main/image/witch.png" width = "50%" height="50%" alt=""/>

### Release版本
在"release"文件夹中包含了可运行程序，将其下载到本地并运行source文件中的"SoftRenderQt.exe"即可体验。

### 相机控制
* 按住鼠标左键并拖动以实现旋转
* 滚动鼠标滚轮切换Zoom(轨道相机的直径)大小
* 按住滚轮并拖动以改变中心点

### 参考
* [GAMES101](http://games-cn.org/intro-graphics/)
* [tinyrenderer](https://github.com/ssloy/tinyrenderer)
* [Software Renderer](https://github.com/zauonlok/renderer)
* [SRender](https://github.com/SunXLei/SRender)
