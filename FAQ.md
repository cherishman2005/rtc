
# FAQ

## webrtc统计数据

chrome://webrtc-internals/


## volume 视频音量设置

将视频音量设置为 20%：
```javascript
myVid = document.getElementById("video1");
myVid.volume = 0.2;
```

## h264分辨率和码率对应关系

h264编码推荐的分辨率和码率关系如下:

| 分辨率      | 码率                 | 
| --------- |  --------------------------- |
| 320x240               |     200-384kbps          | 
| 640x480               |     768-1024kbps         |
| 1280x720(720p)        |     2048-3072kbps        |
| 1920x1080(1080p)      |     5120-8192kbps        |
