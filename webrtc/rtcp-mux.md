# rtcp-mux

RTCP-MUX（RTCP Multiplexing）是一种技术，用于在单个端口上同时传输 RTP 数据包和 RTCP 控制包。这样可以减少网络传输中的端口占用，并简化音视频流传输中的网络配置。在实际应用中，RTCP-MUX 通常与 RTP 数据流一起传输，以提高网络效率。

在音视频传输中启用 RTCP-MUX，可以通过以下方法之一实现：

使用支持 RTCP-MUX 的协议和软件：确保您的音视频传输协议和相关软件（如编码器、解码器等）支持 RTCP-MUX 功能。在配置时开启该选项，或者检查文档以了解如何启用 RTCP-MUX。

在 SDP（Session Description Protocol）中包含 RTCP-MUX 属性：在 SDP 描述中添加 a=rtcp-mux 属性，表示启用 RTCP-MUX。FFmpeg 在转推音视频流时会根据 SDP 中的描述来配置相应的属性。

使用 FFmpeg 命令行参数：在 FFmpeg 命令行中添加参数 -rtcp-mux，以指定启用 RTCP-MUX 功能。例如：

```Shell
ffmpeg -i input-stream -c:v copy -c:a copy -f rtp -rtcp-mux output-stream
```
以上是一些常见的方式来启用 RTCP-MUX 功能，具体应根据您的实际用例和软件环境来选择适合的方法。


# 小结

* 网络传输中 端口是稀缺资源。
* 怎样确定rtcp-mux生效？
