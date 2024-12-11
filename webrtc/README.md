# webrtc

- [rtp-h264](/webrtc/rtp-h264.md)

![webrt-opensource](./webrtc-opensource.png)

![image](https://github.com/user-attachments/assets/8b2f5517-7c18-43da-88ce-5be8261dc5ec)

## webrtc协议栈

webrtc并不是单一的协议，包含了媒体、加密、传输层等在内的多个协议标准以及一套基于 JavaScript 的 API，它包括了音视频的采集、编解码、网络传输、显示等功能。通过简单易用的 JavaScript API ，在不安装任何插件的情况下，让浏览器拥有了 P2P音视频和数据分享的能力。

![image](https://github.com/user-attachments/assets/3703be52-46fb-4347-a721-97057d008da1)


![image](https://github.com/user-attachments/assets/3176efe7-d34a-49b4-9597-6e2b2e87170f)

## webrtc技术框架

![image](https://github.com/user-attachments/assets/522a9d2f-292b-45b2-b699-48eacb7f1d4b)


## webrtc P2P视频通话

![image](https://github.com/cherishman2005/rtc/assets/17688273/34d3aa73-db48-4d91-9305-62c03200da85)

## 基于ffmpeg c++ 后端 音视频处理工具

![2416242a2585740da599ca5b3652447](https://github.com/cherishman2005/rtc/assets/17688273/e383e51c-633c-448a-a16e-8126e25b3ca9)

# FAQ

* 卡顿
* 花屏

## RTP SDP（Session Describe Protocol），会话描述协议 能否携带token鉴权信息？

* 可以自定义字段，携带token信息

## webrtc 2路流混流，有时会非常模糊，有什么好的解决方法？

* 如果是同步虚拟摄像头播放的，可能片源就是低画质的；然后在画面放大的情况下播放。

## webrtc推流时怎样设置固定宽高？

* webrtc推流过程中，宽高 会发生变化。webrtc推流时怎样设置固定宽高？
* 视频编码时 分辨率发生变化 怎么办？

### webrtc分辨率分析

chrome://webrtc-internals/

![image](https://github.com/user-attachments/assets/0343e15d-becc-487e-b0df-f6e99f325131)

## webrtc 点对点视频如何固定码率传输 达到视频播放分辨率不变的效果？

设置了720p 1080p 分辨率 在网络抖动的情况下 分辨率发生了变话 如何让分辨率不变呢？

* 方法1：RTCConfiguration enableCpuOveruseDetection=false 关闭分辨率变换；
* 方法2：修改源码，调整固定帧率、定分辨率、自适应调整策略；


# 参考链接

- [https://web.dev/articles/webrtc-basics?hl=zh-cn](https://web.dev/articles/webrtc-basics?hl=zh-cn)

- [解决ffmpeg拉流转发频繁丢包问题max delay reached. need to consume packet](https://www.cnblogs.com/tc310/p/11218236.html)

- [https://www.nxrte.com/jishu/webrtc/11920.html](https://www.nxrte.com/jishu/webrtc/11920.html)

- [浅谈WebRTC技术原理与应用](https://juejin.cn/post/7049971140474699789)
