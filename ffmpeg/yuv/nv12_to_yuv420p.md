# NV12视频格式与YUV420P格式转化

视频raw data格式分为YUV和RGB格式，常见的YUV 格式有YUV420、YUV422、YUV444等，常见的RGB格式有RGB和RGBA等，根据内存存储方式的不同，YUV420又分为YUV420P和YUV420SP，分别是3平面存储和2平面存储；

常见的NV12、NV21、I420、YV12等都属于YUV420；FFMPEG中YUV420P与I420相同，本文从存储方式选取NV12和YUV420P两种格式做转换处理。

当然如果不想了解这些，使用ffmpeg中libswscale/swscale.h 中的swscal函数可以实现任意格式转化。

1.NV12转换为YUV420P

![image](https://user-images.githubusercontent.com/17688273/147182108-edfde34e-dd4c-4154-b280-2f57253d348a.png)

2.YUV420P转换为NV12

![image](https://user-images.githubusercontent.com/17688273/147182162-6f86a145-7a97-4560-aa59-337732d738e4.png)

注：NV12将数据存储中第二个平面的UV数据交换即可得到NV21格式，因此YUV420P与NV21的转化也很简单。
