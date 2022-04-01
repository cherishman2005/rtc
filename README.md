# RTC


## rtc技术介绍

- [rtc技术介绍](rtc.md)
- [音视频基础技术](doc/av/README.md)

## ffmpeg

基于ffmepg-sdk开发音视频编码/解码相关后端程序，重点就是要多读ffmpeg源码（前提是C/C++编程基础）。

- [FFmpeg开发入门介绍](https://github.com/cherishman2005/rtc/wiki/FFmpeg%E5%BC%80%E5%8F%91%E5%85%A5%E9%97%A8%E4%BB%8B%E7%BB%8D)
- [ffmpeg部分源码解读](doc/ffmpeg部分源码解读.md)
- [ffmepg抽帧截图](ffmpeg/ffmepg抽帧截图.md)
- [webrtc ffmpeg录制、转码](ffmpeg/ffmpeg_cmd.md)

### gpu硬件解码

显卡解码

对于解码来讲相对简单一些，解码性能最主要看两个指标就可以了，一个是单解码器解码的帧数，一个是解码芯片数。

## YUV

- [YUV与RGB转换](https://github.com/cherishman2005/rtc/wiki/YUV%E4%B8%8ERGB%E8%BD%AC%E6%8D%A2)

### YUV各种格式转换

* yuv420sp（I420）

* yuv420p

* NV12

* NV21

## RTMP

- [RTMP](doc/av/RTMP.md)

## obs推流

- [obs推流](https://github.com/cherishman2005/rtc/wiki/obs%E6%8E%A8%E6%B5%81)


# 小结

对于中小型公司，做音视频直播，视频会议相关的产品，自研 UDP，P2P 是不太可能的，运营这样一个研发团队，一年的开销是千万级别的投入。所以 webrtc，甚至第三方基于 webrtc开发的sdk，成了中小公司进入音视频领域的最快做产品的方案。未来 WebRTC 在国内的应用，也会越来越流行，需要大量的开发者。

# FAQ

* webrtc录制的视频 出现先模糊，后清晰的过程。有没有什么优化方法，让webrtc接入 一开始就是清晰视频。


# 参考链接

- [FFmpeg 音视频开发 20 年](https://jishuin.proginn.com/p/763bfbd5802d)
