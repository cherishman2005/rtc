# live-architecture直播系统架构

![直播系统架构](/img/live_architecture.jpg)


## 推流

rtmp

## 录制

flv/mp4/webm

## 拉流

* rtmp
* http-flv
* hls

## 调度

* HTTPDNS


## 直播CDN: 

1. 一般采用nginx-rtmp/SRS开源或自研服务器
1. 直接接入第三方CDN

拉流格式

| Name      | Description                 |      时延        |                             |
| --------- |  --------------------------- |--------------------------- | --------------------------- |
| rtmp      | tcp 流，格式：flv，连续流  | 1~3s                   | 1935端口                   |
| http-flv  | http 流，格式：flv，连续流 | 1~3s                   | 80/443端口，灵活支持        |
| HLS       | http，格式：TS 文件       | 5~20s(一切片情况)       |  时延较大                   |

【注】一般不用支持dash的拉流


# Author
zhangbiwu