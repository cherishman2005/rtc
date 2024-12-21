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

## 二、Licode

Licode 既可以用作 SFU 类型的流媒体服务器，也可以用作 MCU 类型的流媒体服务器。一般情况下，它都被用于 SFU 类型的流媒体服务器。

Licode 不仅仅是一个流媒体通信服务器，而且还是一个包括了媒体通信层、业务层、用户管理等功能的完整系统，并且该系统还支持分布式部署。

Licode 是由 C++ 和 Node.js 语言实现。其中，媒体通信部分由 C++ 语言实现，而信令控制、用户管理、房间管理用 Node.js 实现。它的源码地址为：
https://github.com/lynckia/licode 。下面这张图是 Licode 的整体架构图：

![image](https://github.com/user-attachments/assets/ba9acb77-2075-4bbe-86c4-8812dcd66f24)

通过这张图你可以看出，Licode 从功能层面来讲分成三部分，即 Nuve 、ErizoController 和 ErizoAgent 三部分，它们之间通过消息队列进行通信。

* Nuve 是一个 Web 服务，用于管理用户、房间、产生 token 以及房间的均衡负载等相关工作。它使用 MangoDB 存储房间和 token 信息，但不存储用户信息。
* ErizoController，用于管理控制，信令和非音视频数据都通过它接收。它通过消息队列与 Nuve 进行通信，也就是说 Nuve 可以通过消息队列对 ErizoController 进行控制。
* ErizoAgent，用于音视频流媒体数据的传输，可以分布式布署。ErizoAgent 与 ErizoController 的通信也是通过消息队列，信令消息通过 ErizoController 接收到后，再通过消息队列发给 ErizoAgent，从而实现对 ErizoAgent 进行控制。

icode 不仅仅是一个 SFU 流媒体服务器，它还包括了与流媒体相关的业务管理系统、信令系统、流媒体服务器以及客户端 SDK 等等，可以说它是一个比较完善的产品。

如果你使用 Licode 作为流媒体服务器，基本上不需要做二次开发了，所以这样一套系统对于没有音视频积累的公司和个人具有非常大的诱惑力。目前 Intel CS 项目就是在 Licode 基础上研发出来的，已经为不少公司提供了服务。

官网提供学习demo，和文档。

但 Licode 也有以下一些缺点：
* github star 2.4k issue和pr相当活跃，社区采用的是传统提问，及时沟通相对较差
* 在 Linux 下目前只支持 Ubuntu 14.04 版本，在其他版本上很难编译通过。
* Licode 不仅包括了 SFU，而且包括了 MCU，所以它的代码结构比较重，学习和掌握它要花不少的时间。
* Licode 的性能一般， 如果你把流媒体服务器的性能排在第一位的话，那么 Licode 就不是特别理想的 SFU 流媒体服务器了。
* 官方没有看到android和ios的SDK，有其他人实现，但是早已经不更新，如果你要考虑安卓和ios的话，可能自己会下功夫。

## 三、Janus-gateway

Janus 是一个非常有名的 WebRTC 流媒体服务器，它是以 Linux 风格编写的服务程序，采用 C 语言实现，支持 Linux/MacOS 下编译、部署，但不支持 Windows 环境。

它是一个开源项目，其源码的编译、安装非常简单，只要按 GitHub 上的说明操作即可。源码及编译手册的地址为：
https://github.com/meetecho/janus-gateway 。

Janus 的部署也十分简单，具体步骤详见文档，地址为：

https://janus.conf.meetecho.com/docs/deploy.html 。

![image](https://github.com/user-attachments/assets/36768394-a50a-4e1d-9954-a3b9ca079633)

从上面的架构图中，你可以看出 Janus 分为两层，即应用层和传输层。

插件层又称为应用层，每个应用都是一个插件，可以根据用户的需要动态地加载或卸载掉某个应用。插件式架构方案是非常棒的一种设计方案，灵活、易扩展、容错性强，尤其适用于业务比较复杂的业务，但缺点是实现复杂，成本比较高。

在 Janus 中默认支持的插件包括以下几个。

* SIP：这个插件使得 Janus 成了 SIP 用户的代理，从而允许 WebRTC 终端在 SIP 服务器（如 Asterisk）上注册，并向 SIP 服务器发送或接收音视频流。
* TextRoom：该插件使用 DataChannel 实现了一个文本聊天室应用。
* Streaming：它允许 WebRTC 终端观看 / 收听由其他工具生成的预先录制的文件或媒体。
* VideoRoom：它实现了视频会议的 SFU 服务，实际就是一个音 / 视频路由器。
* VideoCall：这是一个简单的视频呼叫的应用，允许两个 WebRTC 终端相互通信，它与 WebRTC 官网的例子相似（ https://apprtc.appspot.com ），不同点是这个插件要经过服务端进行音视频流中转，而 WebRTC 官网的例子走的是 P2P 直连。
* RecordPlay：该插件有两个功能，一是将发送给 WebRTC 的数据录制下来，二是可以通过 WebRTC 进行回放。

传输层包括媒体数据传输和信令传输。媒体数据传输层主要实现了 WebRTC 中主要有流媒体协议及其相关协议，如 DTLS 协议、ICE 协议、SDP 协议、RTP 协议、SRTP 协议、SCTP 协议等。

信令传输层用于处理 Janus 的各种信令，它支持的传输协议包括 HTTP/HTTPS、WebSocket/WebSockets、NanoMsg、MQTT、PfUnix、RabbitMQ。不过需要注意的是，有些协议是可以通过编译选项来控制是否安装的，也就是说这些协议并不是默认全部安装的。另外，Janus 所有信令的格式都是采用 Json 格式。

Janus 整体架构采用了插件的方案，这种架构方案非常优秀，用户可以根据自己的需要非常方便地在上面编写自己的应用程序。

而且它目前支持的功能非常多，比如支持 SIP、 RTSP、音视频文件播放、录制等等，所以在与其他系统的融合性上有非常大的优势。

另外，它底层的代码是由 C 语言编写的，性能也非常强劲。Janus 的开发、部署手册也非常完善，因此它是一个非常棒的开源项目。

github star4.1k,并且处理issue和pr相对较快。

官方提供安卓和ios的sdk。

缺点：
* 架构太复杂，不适合初学者，公司采用的话人力成本和时间成本会比较高
* janus 底层没有使用 epoll 这类异步I/O事件处理机制，这应该说是它的一大缺陷
* Janus还使用 glib 库，由于 glib 库对于国内的很多开发同学来说用的比较少，所以会有一定的学习成本

## 四、Medooze

Medooze 是一款综合流媒体服务器，它不仅支持 WebRTC 协议栈，还支持很多其他协议，如 RTP、RTMP 等。其源码地址为：
https://github.com/medooze/media-server

![image](https://github.com/user-attachments/assets/4a20aaa4-8f07-4eab-b821-c0517020d135)

从大的方面来讲，Medooze 支持 RTP/RTCP、SRTP/SRCP 等相关协议，从而可以实现与 WebRTC 终端进行互联。除此之外，Medooze 还可以接入 RTP 流、RTMP 流等，因此你可以使用 GStreamer/FFmpeg 向 Medooze 推流，这样进入到同一个房间的其他 WebRTC 终端就可以看到 / 听到由 GStream/FFmpeg 推送上来的音视频流了。另外，Medooze 还支持录制功能，即上图中的 Recorder 模块的作用，可以通过它将房间内的音视频流录制下来，以便后期回放。

Medooze 的控制逻辑层是通过 Node.js 实现的，Medooze 通过 Node.js 对外提供了完整的控制逻辑操作相关的 API，通过这些 API 你可以很容易的控制 Medooze 的行为了。

Medooze 与 Mediasoup 相比，两者在核心层实现的功能都差不多，但 Medooze 的功能更强大，包括了录制、推 RTMP 流、播放 FLV 文件等相关的操作，而 Mediasoup 则没有这些功能。

Medooze 也有一些缺点，尽管 Medooze 也是 C++ 开发的流媒体服务务器，使用了异步 IO 事件处理机制，但它使用的异步 IO 事件处理的 API 是 poll，poll 在处理异步 IO 事件时，与 Linux 下最强劲的异步 IO 事件 API epoll 相比要逊色不少，这导致它在接收 / 发送音视频包时性能比 Mediasoup 要稍差一些。

## 五、jitsi

使用Java构建的服务端，底层也是使用c/c++,使用Java语言所以性能上没有使用c/c++的表现好。

主要模块及实现语言：
* Jitsi Video-Bridge (Software video-bridge 实现语言java)
* Jitsi Jicofo (Component mandatory for jitsi conference 实现语言java)
* Prosody ( XMPP Server 实现语言lua)
* Nginx (Web Server)
* Jitsi Meet (Web application – to which the end user will interact. 实现语言js)

优点：
* github star12.3k，issue和pr处理快
* 文档齐全
* 官方提供安卓和ios SDK，也可以自行编译SDK，使用的是React Native
* 官方提供web端的SDK，并提供使用electron进行桌面端打包（端很齐全）
* 社区采用论坛方式沟通，活跃较高
* 社区提供分布式解决方案，但是文档偏少。
* 每周一维护团队在jitsi上进行视频会议，回答开发者的提问，沟通使用英文，国内时间好像是晚上。
* 社区版本更新迭代较快

## 六、Kurento

Kurento和jitsi是一样，持续维护了很多年，经过了时间的检验。不同的是他是使用c++开发，有丰富的文档和示例裤，对于开发者来说非常友好。

## 七、pion/webrtc

WebRTC API的Pure Go实现，github上star4.7k，目前用的人较少，不建议使用在产品环境，可以学习参考使用，建议长期关注。

# 总结：

对SFU流媒体服务器的选择，没有最好，只有最合适。每个开源实现都有其各自的特点，都可以应用到实际产品中，只不过作为开发人员都有自己独特的技术背景，你需要根据自身特点以及项目特点选一个最合适的。接下来，我就介绍一下我是如何对这些开源项目进行评判和选择的。

团队 在一个团队中肯定会选择一种大家都比较熟悉的语言作为项目开发的语言，所以我们在选择开源项目时，就要选择使用这种语言开发的开源项目。 比如阿里系基本都用 Java 语言进行开发，所以它们在选择开源项目时，基本都会选择 Java 开发的开源项目； 而做音视频流媒体服务的开发人员，为了追求性能，所以一般都选择 C/C++ 语言开发的开源项目。 团队人手如果不充裕的情况下，尽量就不要选择特别复杂的，和文档比较少的开源技术。

适合业务 要充分考虑到你的业务的用户量和用户群体，如果你的业务量很大，需要做分布式，那么你选择的开源技术一定要先去了解下他是否支持分布式部署，分布式部署采用那种方式。单机支持多少并发，最好自己用服务器实际测试下，官方数据会和实际测试数据多少都有出入。 项目功能也需要考虑，比如业务需要录制回放，开源技术并没有这样的功能，需要自己开发，时间成本很高，但选择已经做好录制回放功能的开源技术又不一样了。

二次开发 Licode 是一个完整的系统，支持分布式集群部署，所以系统相对复杂，学习周期要长一些。它可以直接布署在生产环境，但是二次开发的灵活性不够。 Janus-gateway 是一个独立的服务，支持的信令协议很丰富，而且支持插件开发，易扩展，对于 Linux/C 背景的开发者是很不错的选择。 Medooze 和 Mediasoup 都是流媒体服务器库，对于需要将流媒体服务器集成到自己产品中的开发者来说，应该选择它们。

时间成本 公司对于项目的时间计划和成本也要考量，因为使用开源技术或多或少都会遇到坑，有可能一个坑会卡很久，所以使用文档全，社区活跃的开源技术比较好。

无论选择哪种开源技术，前期一定要做好调研，并实际自己搭建使用过在做决定，选择好后，为了弥补技术债，还需要去深入理解开源技术的代码，不然还债的时候很疼苦。

# 参考链接

- [当前开源WebRTC项目技术选型](https://www.toutiao.com/article/7267852090779746816/)

