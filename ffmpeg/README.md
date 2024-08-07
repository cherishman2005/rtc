# ffmpeg

![image](https://github.com/user-attachments/assets/4d83422b-0b80-4949-a068-7ca9a11451c0)

## ffmpeg常见使用

- [ffmpeg常见使用](./ffmpeg常见使用.md)

- [ffmpeg基础操作](./ffmpeg基础操作.md)


## 码率控制

视频编码时进行码率控制bit-rate，在同等视频质量的情况下节约带宽；

## ffmpeg extra_data

引用AVCodecContext中对该数据成员的解释
```
/**
     * some codecs need / can use extradata like Huffman tables.
     * MJPEG: Huffman tables
     * rv10: additional flags
     * MPEG-4: global headers (they can be in the bitstream or here)
     * The allocated memory should be AV_INPUT_BUFFER_PADDING_SIZE bytes larger
     * than extradata_size to avoid problems if it is read with the bitstream reader.
     * The bytewise contents of extradata must not depend on the architecture or CPU endianness.
     * - encoding: Set/allocated/freed by libavcodec.
     * - decoding: Set/allocated/freed by user.
     */
    uint8_t *extradata;
    int extradata_size;
```
可见它针对不同的情况有不同的格式，而比较常用的情况就是我们对视频流进行写入文件操作时（某些情况下，如通过NV12格式编码的视频流数据），或则解码视频文件时需要我们去设置。此时extradata作为一个global headers，主要保存SPS、PPS等信息，下面就针对此种情况进行说明。


## ffmpeg性能压测

## 多个输入连接到单个输出文件

```
/usr/bin/ffmpeg -i vtest.mp4 -i vtest.mp4 -y -filter_complex concat=n=2:v=1:a=0 out.mp4
```

## ffmpeg多路转码
```
root      1522  1498 99 15:21 pts/1    00:04:08 /usr/bin/ffmpeg -i /data12/nodejs/video/win.HD.1080p.mp4 -y -acodec aac -strict experimental -vcodec libx264 -filter:v scale=w=640:h=400 -f mp4 /data12/nodejs/video/output-medium.mp4
root      1523  1498 99 15:21 pts/1    00:03:08 /usr/bin/ffmpeg -i /data12/nodejs/video/win.HD.1080p.mp4 -y -acodec aac -strict experimental -vcodec libx264 -filter:v scale=w=320:h=200 -f mp4 /data12/nodejs/video/output-small.mp4
root      1524  1498 99 15:21 pts/1    00:06:45 /usr/bin/ffmpeg -i /data12/nodejs/video/win.HD.1080p.mp4 -y -acodec aac -strict experimental -vcodec libx264 -f mp4 /data12/nodejs/video/output-original-size.mp4
```

ffmpeg软件转码太耗cpu
![image](https://user-images.githubusercontent.com/17688273/148635803-b4a17637-7445-4469-92b2-88d1bb600948.png)

## opencv+ai人脸检测

* opencv预处理
* AI-model process
* post后处理

![image](https://user-images.githubusercontent.com/17688273/148714857-163e7e23-bb23-4566-a1df-a4685d84c46f.png)

运行示例结果：

![image](https://user-images.githubusercontent.com/17688273/148715081-8cec06e5-e162-4fc6-bbc9-1d1c03a68e47.png)


### Facemark：使用OpenCV进行面部特征点检测

### 音视频特效

（1）人脸检测 -> ROI感知编码；

（2）facemark加特效；

### ffmpeg -threads

```
-threads 0 (optimal);

-threads 1 (single-threaded);

-threads 2 (2 threads for e.g. an Intel Core 2 Duo);

none (the default, also optimal).
```

# FAQ

## ffmpeg抽帧截图

使用ffmpeg对视频均匀抽帧，但是发现占用的cpu和内存挺多的，cpu这边好像可以通过指定参数 -threads n 来进行多线程处理，但是最多降一半，内存就没有办法降了，我原本以为是可以减少加载的模块来减少cpu和内存，但是找了一圈没有发现类似的回答。看到您的认证是图像算法工程师，所以想请教下你在工作中有碰到类似的场景吗？

抽关键帧，讲道理不应该很占资源。识别I帧只需要读nalu头部就成，都不需要解码和重编码。抽取B、P帧重新组成视频倒有可能。

# 参考链接

- [使用ffmpeg命令行实现一入多出转码](https://blog.csdn.net/xiaoluer/article/details/81346285)
- [ffmpeg extra_data](https://blog.csdn.net/a812073479/article/details/74721119)

- [opencv opencv4nodejs 安装和简单抠图](https://www.codenong.com/cs122064921/)

- [ffmpeg中的roi encoding介绍](https://blog.csdn.net/yjguo2004/article/details/104460763)
- [https://github.com/pogofdev/faceRecognitionDemo](https://github.com/pogofdev/faceRecognitionDemo)

- [https://github.com/DominicCabral/face-recognition](https://github.com/DominicCabral/face-recognition)

- [https://www.geeksforgeeks.org/how-to-use-opencv-with-node-js/](https://www.geeksforgeeks.org/how-to-use-opencv-with-node-js/)

- [c++ - 使用OpenCV将YUV转换为有损压缩](https://www.coder.work/article/3325452)

- [OpenCV实战：人脸关键点检测（FaceMark）](http://www.yaowenming.com/A/QV5ZXVDyzy/)

- [基于主观感兴趣区域的视频编码实践](https://cloud.tencent.com/developer/article/1676158)

- [https://github.com/runner365/srt_encoder/wiki/How-to-compile-cn](https://github.com/runner365/srt_encoder/wiki/How-to-compile-cn)
- [FFmpeg视频抽帧那些事](https://zhuanlan.zhihu.com/p/85895180)
