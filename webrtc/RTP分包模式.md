# RTP分包模式

RTP（Real-time Transport Protocol，实时传输协议）的分包模式指的是在传输媒体数据时将其拆分为 RTP 数据包的方式。RTP 数据包是用于传输实时音视频数据的基本单位，通过选择不同的分包模式可以影响数据包的大小、传输效率、延迟等方面。

以下是一些常见的 RTP 分包模式：

1. 单数据包模式（Single Packet Mode）：
* 将完整的音频或视频帧打包成一个 RTP 数据包进行传输。
* 适用于要求低延迟和带宽的实时通信场景。

2. 分片数据包模式（Fragmentation Packet Mode）：
* 将较大的音频或视频帧拆分为多个 RTP 数据包进行传输。
* 可以降低单个数据包的大小，减少丢包率和传输延迟。

3. 聚合数据包模式（Aggregation Packet Mode）：
* 将多个小的音频或视频帧聚合到一个 RTP 数据包中进行传输。
* 适用于需要优化网络负载和减少数据包数量的场景。

4. 交错数据包模式（Interleaved Packet Mode）：
* 轮流发送不同媒体流的 RTP 数据包，以便在接收端重新组装数据。
* 适用于同时传输多个媒体流的场景，如音频和视频同时传输。

选择合适的 RTP 分包模式取决于具体的应用需求，包括带宽限制、延迟要求、数据传输稳定性等方面。不同的分包模式会对传输效率、延迟和数据包大小产生不同的影响，因此需要根据具体情况进行选择。

在实际应用中，根据需要可能会采用不同的分包模式来优化音视频数据的传输效果。具体的分包模式选择可以根据现有的标准协议和实践经验进行调整。

## mediasoup PacketizationMode

在 mediasoup 中，PacketizationMode（打包模式）用于确定如何将媒体数据打包成 RTP（Real-time Transport Protocol）数据包。
主要有两种打包模式：

1. PacketizationMode.SingleNalUnit：
* 这种模式下，每个 NAL（Network Abstraction Layer）单元（例如 H.264 或 H.265 的编码数据块）被封装在一个单独的 RTP 数据包中。
* 优点是简单直接，处理起来相对容易。对于低延迟的场景可能比较适用，因为数据包较小，发送和处理速度可能更快。
* 缺点是可能会产生更多的数据包，增加网络开销。

2. PacketizationMode.NonInterleaved：
* 此模式允许将多个 NAL 单元打包在一个 RTP 数据包中。
* 优点是可以减少网络数据包的数量，从而降低网络开销。对于网络带宽有限的情况可能更有优势。
* 缺点是处理起来相对复杂一些，可能会增加一定的延迟，因为需要等待多个 NAL 单元才能打包成一个数据包。

在使用 mediasoup 时，可以根据具体的应用场景和需求选择合适的PacketizationMode。例如，如果对延迟要求非常高，可以选择SingleNalUnit模式；如果网络带宽是主要考虑因素，可以考虑NonInterleaved模式。
以下是一个设置打包模式的示例代码（假设在 mediasoup 的特定上下文中）：

```javascript
const producer = await producerTransport.produce({
  kind: 'video',
  rtpParameters: {
    // 设置打包模式为 SingleNalUnit
    packetizationMode: 'SingleNalUnit',
    // 其他参数...
  }
});
```
你可以根据实际情况调整打包模式和其他相关参数，以满足你的应用需求。
