# 开源WebRTC项目技术选型

## 前言

当前Webrtc技术已经成熟，各大公司都在自己的产品中集成Webrtc功能，Webrtc除了要实现视音频通信外还有需要实现信令传输，而Webrtc的核心模块就是流转发模块，即流媒体服务器，如果要自已实现流媒体服务器困难还是比较大的，付出的时间成本也将不少，它里面要涉及到去研究 DTLS 协议、ICE 协议、SRTP/SRTCP 协议等方面，光理解这些协议就要花不少的时间，更何况要去实现它了，所以最快捷的办法就是使用开源的实现。本文就开源的几个webrtc做下介绍比较。

如果您要快速的搭建起音视频会议系统的话，使用开源技术解决方案无疑是最快的办法，最能节省人力成本与时间成本的。下面分析常见的视频会议的核心模块（流转发服务器）SFU开源解决方案的优缺点，以便你在选择合适的开源解决方案提供参考。

## 一、Mediasoup

Mediasoup 是推出时间不长的 WebRTC 流媒体服务器开源库，其地址为：
https://github.com/versatica/mediasoup 。

Mediasoup 由应用层和数据处理层组成。应用层是通过 Node.js 实现的；数据处理层由 C++ 语言实现，包括 DTLS 协议实现、ICE 协议实现、SRTP/SRTCP 协议实现、路由转发等。

![image](https://github.com/user-attachments/assets/f8f8cf4f-653e-4829-8c81-c2b65cf1ab24)

Mediasoup 把每个实例称为一个 Worker，在 Worker 内部有多个 Router，每个 Router 相当于一个房间。在每个房间里可以有多个用户或称为参与人，每个参与人在 Mediasoup 中由一个 Transport 代理。换句话说，对于房间（Router）来说，Transport 就相当于一个用户。

Transport 有三种类型，即 WebRtcTransport、PlainRtpTransport 和 PipeTransport。
* WebRtcTransport 用于与 WebRTC 类型的客户端进行连接，如浏览器。
* PlainRtpTransport 用于与传统的 RTP 类型的客户端连接，通过该 Transport 可以播放多媒体文件、FFmpeg 的推流等。
* PipeTransport 用于 Router 之间的连接，也就是一个房间中的音视频流通过 PipeTransport 传到另一个房间。

在每个 Transport 中可以包括多个 Producer 和 Consumer。
* Producer 表示媒体流的分享者（或推流端），它又分为两种类型，即音频的共享者和视频的分享者。
* Consumer 表示媒体流的消费者（或订阅端），它也分为两种类型，即音频的消费者和视频的消费者。

Mediasoup 的实现逻辑非常清晰，它不关心上层应用该如何做，只关心底层数据的传输，并将它做到极致。

Mediasoup 底层使用 C++ 开发，使用 libuv 作为其异步 IO 事件处理库，所以保证了其性能的高效性。同时它支持了几乎所有 WebRTC 为了实时传输做的各种优化，所以说它是一个特别优秀的 WebRTC SFU 流媒体服务器。它与 Janus 相比，它更聚焦于数据传输的实时性、高效性、简洁性，而 Janus 相比 Mediasoup 做的事儿更多，架构和逻辑也更加复杂。

`对于开发能力比较强的公司来说，根据自己的业务需要在 Mediasoup 上做二次开发也是非常值得推荐的技术方案。`

手机端的话需要自己实现安卓和ios的SDK。


