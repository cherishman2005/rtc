# rtc应用场景

在线教育全场景解决方案8大场景：

	1v1教学
	1对多小班课
	互动大班课
	双师课堂
	游戏化教学
	在线音乐教学
	AI互动课堂
	超级小班课

# webrtc

WebRTC由三大块组成：

![WebRTC的组成](/img/webrtc.png)

	（1）getUserMedia是负责获取用户本地的多媒体数据，如调起摄像头录像等。
	（2）RTCPeerConnection是负责建立P2P连接以及传输多媒体数据。
	（3）RTCDataChannel是提供的一个信令通道。


## Mesh (P2P)

WebRTC P2P模式下的网络拓扑结构

![P2P](/img/webrtc-p2p.png)

![Mesh](/img/webrtc-mesh.png)


## SFU

SFU全称：Selective Forwarding Unit。

![SFU](/img/webrtc-sfu.png)

## MCU

![MCU](/img/webrtc-mcu.png)


# Author

zhangbiwu

- [阿武的博客](https://cherishman2005.github.io/)

主要技术栈：

* 后端研发
  * nginx/openresty
  * C/C++
  * nodejs
  * js-sdk
  * RTM/RTC
