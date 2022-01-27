# ffmpeg抽帧截图

（1）在一个时间节点截图：
```
ffmpeg  -ss 00:01:00 -i /data/services/nodejs/video/test.mp4 -f image2  -vf fps=fps=1/60 -qscale:v 2 /data/services/nodejs/video/test/test-%05d.jpeg
```

（2）-t 代表持续时间，单位为秒：
```
ffmpeg  -ss 00:01:00 -i /data/services/nodejs/video/test.mp4 -f image2  -t 10 -qscale:v 2 /data/services/nodejs/video/test/test-%05d.jpeg
```

（3） 抽取关键帧
```
ffmpeg -i /data/services/nodejs/video/test.mp4 -an -vf select='eq(pict_type\,I)' -vsync 2 -s 544*960 -f image2 /data/services/nodejs/video/test_images/test-%03d.jpg
```

各参数解释:

-i :输入文件，这里的话其实就是视频；

-vf:是一个命令行，表示过滤图形的描述。选择过滤器select会选择帧进行输出：pict_type和对应的类型:PICT_TYPE_I 表示是I帧，即关键帧；

-vsync 2:阻止每个关键帧产生多余的拷贝；

-f image2 name_%02d.jpeg:将视频帧写入到图片中，样式的格式一般是: “%d” 或者 “%0Nd”

-s:分辨率，544*960

这样保存下来的关键帧的命名顺序是从001开始的，数字表示第几个关键帧。

```
ffmpeg -i /data/services/nodejs/video/test.mp4 -an -vf select='eq(pict_type\,I)' -vsync 2 -f image2 /data/services/nodejs/video/test_images/test-%03d.jpg
```

（3）视频旋转90度：
```
ffmpeg -i /data/services/nodejs/video/test.mp4 -c copy -metadata:s:v:0 rotate=90 /data/services/nodejs/video/output.mp4
```

## 抽帧截图

抽帧截图是电商、社交业务开发必备功能

**主要功能点**

* 对视频截图、或通过高清图像生成缩略图

* 生成图像后上传的oss对象存储，并将存储的图像对象生成签名url供业务使用

![image](https://user-images.githubusercontent.com/17688273/151287495-70b1ea25-3de1-49ec-99c0-8f61894074fb.png)

# FAQ

## ffmepg抽帧截图出现黑屏怎么解决？

观看了test.mp4视频，视频本来就是采用黑屏切换，抽到I帧黑屏是正常情况。

# 参考链接

- [https://longrm.com/2019/12/19/2019-11-27-ffmpeg-screenshot/](https://longrm.com/2019/12/19/2019-11-27-ffmpeg-screenshot/)

- [https://longrm.com/2019/12/23/2019-12-23-ffmpeg-screenshot-2/](https://longrm.com/2019/12/23/2019-12-23-ffmpeg-screenshot-2/)
