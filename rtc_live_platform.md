# kurento-rtmp连麦互动直播系统架构

![kurento连麦直播系统](/img/kurento-rtmp.png)



## 连麦

RTC： web端一般采用webrtc，移动端采用native

控制信令： websocket/http等协议

## 混流、混画

自研的服务器

【注】简单采用ffmpeg、gstreamer框架


## 直播CDN: 

1. 一般采用nginx-rtmp/SRS开源或自研服务器
1. 直接接入第三方CDN

拉流格式

| Name      | Description                 |                             |
| --------- |  --------------------------- |--------------------------- |
| rtmp      |     tcp 流，格式：flv，连续流  | 1935端口                   |
| http-flv  |     http 流，格式：flv，连续流 | 80/443端口，灵活支持        |
| HLS       |     http，格式：TS 文件        | 时延较大                   |

【注】一般不用支持dash格式的拉流格式


# Author
zhangbiwu