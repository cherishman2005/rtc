# ffmpeg抽帧截图

（1）在一个时间节点截图：
```
ffmpeg  -ss 00:01:00 -i /data/services/nodejs/video/test.mp4 -f image2  -vf fps=fps=1/60 -qscale:v 2 /data/services/nodejs/video/test/test-%05d.jpeg
```

（2）-t 代表持续时间，单位为秒：
```
ffmpeg  -ss 00:01:00 -i /data/services/nodejs/video/test.mp4 -f image2  -t 10 -qscale:v 2 /data/services/nodejs/video/test/test-%05d.jpeg
```

（3）视频旋转90度：
```
ffmpeg -i /data/services/nodejs/video/test.mp4 -c copy -metadata:s:v:0 rotate=90 /data/services/nodejs/video/output.mp4
```

# FAQ

## ffmepg抽帧截图出现黑屏怎么解决？

# 参考链接

- [https://longrm.com/2019/12/19/2019-11-27-ffmpeg-screenshot/](https://longrm.com/2019/12/19/2019-11-27-ffmpeg-screenshot/)

- [https://longrm.com/2019/12/23/2019-12-23-ffmpeg-screenshot-2/](https://longrm.com/2019/12/23/2019-12-23-ffmpeg-screenshot-2/)
