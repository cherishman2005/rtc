# 音视频基础技术

## video封装格式

国内常见的直播协议有几个：RTMP、HLS、HTTP-FLV。

### RTMP

rtmp只要用于直播推流，拉流偏向选用http-flv。

### HLS
- [HLS](HLS.md)


### HTTP-FLV

HTTP-FLV 和 RTMPT 类似，都是针对于 FLV 视频格式做的直播分发流。

但，两者有着很大的区别：

相同点：
* 两者都是针对 FLV 格式
* 两者延时都很低
* 两者都走的 HTTP 通道

不同点：
**HTTP-FLV**

* 直接发起长连接，下载对应的 FLV 文件
* 头部信息简单

**RTMPT**

握手协议过于复杂
分包，组包过程耗费精力大
因为 RTMP 发的包很容易处理，通常 RTMP 协议会作为视频上传端来处理，然后经由服务器转换为 FLV 文件，通过 HTTP-FLV 下发给用户。
