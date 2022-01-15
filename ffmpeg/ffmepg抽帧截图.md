# ffmpeg抽帧截图

```
ffmpeg  -ss 00:01:00 -i /data/services/nodejs/video/test.mp4 -f image2  -vf fps=fps=1/60 -qscale:v 2 /data/services/nodejs/video/test/test-%05d.jpeg
```
