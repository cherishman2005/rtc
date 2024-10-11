# x-google-start-bitrate webrtc

在 WebRTC 中，x-google-start-bitrate 是一个用于设置初始视频比特率的参数。这个参数通常在 SDP（Session Description Protocol）中用于描述视频流的特性，可以确保视频通话在开始时以指定的比特率开始传输。

x-google-start-bitrate 参数可以在 SDP 中的视频描述中进行设置，以指定视频流在开始传输时应该使用的初始比特率。这有助于控制视频质量和带宽的使用，从而提高通话的质量和稳定性。

以下是一个示例SDP中x-google-start-bitrate参数的用法：
```
m=video 9 UDP/TLS/RTP/SAVPF 98
a=rtpmap:98 VP8/90000
a=max-fs:12225
a=rtcp-fb:98 goog-remb
a=rtpmap:100 VP9/90000
...
a=fmtp:98 x-google-start-bitrate=1000
```

在这个示例中，x-google-start-bitrate=1000 表示指定这个视频流的初始比特率为 1000 kbps。

通过设置 x-google-start-bitrate 参数，您可以调整视频通话的初始质量和带宽使用，以满足您的需求和网络条件。
