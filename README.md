# Renderer
---
  一个C++实现的简单光线追踪渲染引擎，实现了局部光照(Local illumination)，全局光照(Global illumination)。主要包括：
  1. RayTrace:  一个简单的光线追踪渲染器，实现了局部光照
  2. PathTrace: 一个简单的光线追踪渲染器，实现全局光照(via unbiased Monte Carlo path tracing)；
  3. PXM tools: 一个简单读写 PPM / PGM / PBM 图像格式的工具；
  4. Image processing: 有关图像处理的工具，如 box blur, median blur, biliteral blur, etc.
  
## 一、Ray tracer
![room](https://raw.githubusercontent.com/zhangpiu/ImageTools/master/pictures/room8h10000x4sample.png "room")

### 1. Local Illumination
&emsp; &ensp; 这里参考了`Milo`大神的两篇博文：[《用JavaScript玩转计算机图形学(一)光线追踪入门》](http://www.cnblogs.com/miloyip/archive/2010/03/29/1698953.html)，[《用JavaScript玩转计算机图形学(二)基本光源》](http://www.cnblogs.com/miloyip/archive/2010/04/02/1702768.html)。在原来的基础上加了一些改进，并用C++予以实现。以下展示一些渲染效果：

#### 灯光渲染

* 平行光光源

* 点光源

* 聚光灯光源
#### 场景渲染

  
### 2. Global Illumination

