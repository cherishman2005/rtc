# FAQ

## 有没有对比分析过freeSwitch与mediasoup?简单评价下mediasoup开源

freeSwitch主要是做MCU。

mediasoup主要是做SFU, mediasoup比较好用，集成了chrome内核。并且用javascript进行控制。开放了一些API，易用性好。在不了解底层代码的前提下，就可以拿来做很多事情。


## 用ffmpeg处理节点js服务器中的WebRTC RTC流

WebRTC需要交换报价/应答。因此ffmpeg需要生成一个返回SDP (答案)。
* ffmpeg没有ICE的实现。
* ffmpeg不知道做DTLS -> SRTP

要执行WebRTC -> RTMP，您可以使用这，后者执行WebRTC，然后像这一样发送到ffmpeg。你可以优化它很多。它通过stdin实现mkv的唯一原因是它可以移植到Windows。您可以执行多个管道，并直接将H 264/Opus发送到其中，如果只执行Linux，则可以节省大量代码。

可以考虑使用GStreamer并执行webrtc -> rtmpsink！


## 开源

如果要用好开源，尽量深层次的分析开源。
