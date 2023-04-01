## SoftRenderQt
<img src="https://github.com/shiyin1014/SoftRenderQt/blob/main/image/phong.png" height="50%" width="50%" alt="" /><img src="https://github.com/shiyin1014/SoftRenderQt/blob/main/image/normal.png" height="50%" width="50%" alt="" />

<img src="https://github.com/shiyin1014/SoftRenderQt/blob/main/image/wire_frame.png" height="50%" width="50%" alt="" /><img src="https://github.com/shiyin1014/SoftRenderQt/blob/main/image/depth.png" height="50%" width="50%" alt="" />
This is a shader based c++ software rasterisation renderer that has no dependencies except for opencv and Qt for the display interface.This project was written to help me better understand computer graphics and related algorithms.

### Fetures
* Blinn-Phong reflection model
* Shader-based
* Perspective correct interpolation
* Back-face culling
* Homogeneous clipping
* Tangent space normal mapping
* Wire frame
* Depth testing
* Orbital camera

### ScreenShots
<img src="https://github.com/shiyin1014/SoftRenderQt/blob/main/image/elfgirl.png" width = "50%" height="50%" alt=""/><img src="https://github.com/shiyin1014/SoftRenderQt/blob/main/image/witch.png" width = "50%" height="50%" alt=""/>



### Build
Opencv and QT environment are required to build this project.
In CMakeLists.txt, you need modify the correct path in your own computer.


### Release
The release version can be found in release folder.
The obj folder contains some models and textures.
You can run it by clicking on "SoftRenderQt.exe" in the "release/source" folder.

### Control of Camera
* Rotate by holding left mouse button
* Zoom (or radius):change by scrolling with mouse wheel
* Holding middle mouse button and moving to change focused point

### Reference
* [GAMES101](http://games-cn.org/intro-graphics/)
* [tinyrenderer](https://github.com/ssloy/tinyrenderer)
* [Software Renderer](https://github.com/zauonlok/renderer)
* [SRender](https://github.com/SunXLei/SRender)
