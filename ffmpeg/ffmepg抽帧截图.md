# ffmpeg抽帧截图

（1）在一个时间节点截图：
```
ffmpeg  -ss 00:01:00 -i /data/services/nodejs/video/test.mp4 -f image2  -vf fps=fps=1/60 -qscale:v 2 /data/services/nodejs/video/test/test-%05d.jpeg
```


# FAQ

## ffmepg抽帧截图出现黑屏怎么解决？

# 参考链接

- [https://longrm.com/2019/12/19/2019-11-27-ffmpeg-screenshot/](https://longrm.com/2019/12/19/2019-11-27-ffmpeg-screenshot/)
