# Renderer
<p align="right"> By zhangpiu </p>
---
  一个C++实现的简单光线追踪渲染引擎，实现了局部光照(Local illumination)，全局光照(Global illumination)。主要包括：
  1. RayTrace:  一个简单的光线追踪渲染器，实现了局部光照
  2. PathTrace: 一个简单的光线追踪渲染器，实现全局光照(via unbiased Monte Carlo path tracing)；
  3. PXM tools: 一个简单读写 PPM / PGM / PBM 图像格式的工具；
  4. Image processing: 有关图像处理的工具，如 box blur, median blur, biliteral blur, etc.
  
## 一、Ray tracer
<p align="center"> <img src="https://raw.githubusercontent.com/zhangpiu/Renderer/master/pictures/room8h10000x4sample.png" alt="room" title="room" /> </p>

### 1. Local Illumination
&emsp; &ensp; 这里参考了`Milo`大神的两篇博文：[《用JavaScript玩转计算机图形学(一)光线追踪入门》](http://www.cnblogs.com/miloyip/archive/2010/03/29/1698953.html)，[《用JavaScript玩转计算机图形学(二)基本光源》](http://www.cnblogs.com/miloyip/archive/2010/04/02/1702768.html)。在原来的基础上加了一些改进，并用C++予以实现。以下展示一些渲染效果：

#### 灯光渲染
* 平行光光源

<p align="center"> <img src="https://raw.githubusercontent.com/zhangpiu/Renderer/master/pictures/directionalLight.png" alt="directionalLight" title="Directional light" /> </p>

* 点光源

<p align="center"> <img src="https://raw.githubusercontent.com/zhangpiu/Renderer/master/pictures/pointLight.png" alt="pointLight" title="Point light" /> </p>

还可以添加多个光源

<p align="center"> <img src="https://raw.githubusercontent.com/zhangpiu/Renderer/master/pictures/36lights.png" alt="36lights" title="36 lights" /> </p>

* 聚光灯光源

<p align="center"> <img src="https://raw.githubusercontent.com/zhangpiu/Renderer/master/pictures/spotLight.png" alt="spotLight" title="Spot light" /> </p>

可以用聚光灯的效果渲染一个调色板

<p align="center"> <img src="https://raw.githubusercontent.com/zhangpiu/Renderer/master/pictures/spotLightRGB.png" alt="spotLightRGB" title="RGB" /> </p>

#### 场景渲染

<p align="center"> <img src="https://raw.githubusercontent.com/zhangpiu/Renderer/master/pictures/spheres.png" alt="spheres" title="Spheres" /> </p>

### 2. Global Illumination

&emsp; &ensp; 这里的全局光照算法是基于Mento Carlo的路径追踪算法，参考了`Kevin Beason`大神的经典之作[《smallpt: Global Illumination in 99 lines of C++》](http://www.kevinbeason.com/smallpt/)，并作了一些改进。但由于加了一些类的封装等，有些对象的拷贝会增加额外的开销，本实现版本的速度其实不及原来的99行的实现，这里不准备再用牺牲可读性来换取时间的优化。

<p align="center"> <img src="https://raw.githubusercontent.com/zhangpiu/Renderer/master/pictures/room8h10000x4sample.png" alt="room" title="room" /> </p>

