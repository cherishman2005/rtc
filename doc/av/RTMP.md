<!-- TOC -->

- [RTMP](#rtmp)
  - [rtmp推流](#rtmp推流)
    - [push数据抓包分析](#push数据抓包分析)
  - [rtmp拉流player](#rtmp拉流player)
    - [pull数据抓包分析](#pull数据抓包分析)
- [FAQ](#faq)
  - [RTMP直播推流中需要注意的点](#rtmp直播推流中需要注意的点)
- [link](#link)

<!-- /TOC -->

# RTMP

## rtmp推流

**tcp 3次握手**

![image](https://user-images.githubusercontent.com/17688273/151367387-033e6e19-cc2c-422f-a708-0aeb1b54af8c.png)


**RTMP握手**

首先服务端与客户端需要通过3次交换报文完成握手,RTMP是由三个静态大小的块,而不是可变大小的块组成的,客户端与服务器发送相同的三个chunk,客户端发送c0,c1,c2,服务端发送s0,s1,s2。

![image](https://user-images.githubusercontent.com/17688273/151366769-4ffb1ea8-3411-47f0-9fbd-4ca2b897a2c1.png)

* 客户端发送 C0，C1 块，握手开始。
* 客户端在发送 C2 之前客户端必须等待接收 S1 。
* 客户端在发送任何数据之前客户端必须等待接收 S2。
* 服务端在发送 S0 和 S1 之前必须等待接收 C0，也可以等待接收 C1。
* 服务端在发送 S2 之前必须等待接收 C1。
* 服务端在发送任何数据之前必须等待接收 C2。

**RTMP建立连接**

![image](https://user-images.githubusercontent.com/17688273/151366968-e16d000c-1ddb-4020-b180-5f832a319d56.png)

* 客户端发送命令消息中的“连接”(connect)到服务器，请求与一个服务应用实例建立连接。
* 服务器接收到连接命令消息后，发送确认窗口大小(Window Acknowledgement Size)协议消息到客户端，同时连接到连接命令中提到的应用程序。
* 服务器发送设置带宽协议(Set Peer Bandwidth)消息到客户端。
* 客户端处理设置带宽协议消息后，发送确认窗口大小(Window Acknowledgement Size)协议消息到服务器端。
* 服务器发送用户控制消息中的“流开始”(Stream Begin)消息到客户端。
* 服务器发送命令消息中的“结果”(_result)，通知客户端连接的状态。


### push数据抓包分析

（1）tcp 3次握手

![image](https://user-images.githubusercontent.com/17688273/151356256-db02b030-0b67-40e5-8f1b-e0083a10ac19.png)


（2）[client -> server] Handshake C0+C1 

![image](https://user-images.githubusercontent.com/17688273/151356966-b274e0bb-cfcb-44f3-8d05-a46aea46d271.png)


（3）[server -> client] Handshake S0+S1+S2

![image](https://user-images.githubusercontent.com/17688273/151357271-78302be9-87f6-46c3-8065-23f252dc3501.png)

（4）[client -> server] Handshake C2

![image](https://user-images.githubusercontent.com/17688273/151357896-8da6a4ef-d340-4050-8613-b3ee06fa7eb5.png)


（5）[client -> server] connect('live') 

![image](https://user-images.githubusercontent.com/17688273/151359231-20b2f87a-cd5c-4986-ad9b-57c27ce5d1a5.png)


（6）[server -> client] Window Acknowledgement Size

![image](https://user-images.githubusercontent.com/17688273/151360346-275a8dfc-810e-4ae5-92a2-02fa6318ff3f.png)

（7）[server -> client] Set Peer Bandwidth(5000000); NetConnection.Connect.Success;

![image](https://user-images.githubusercontent.com/17688273/151361033-413b148d-2258-4299-a698-2f66c259f16e.png)

（8）[client -> server] createStream()

![image](https://user-images.githubusercontent.com/17688273/151361676-024f6a4c-fae1-43b6-b9b7-96ef7f610862.png)

（9）publish

![image](https://user-images.githubusercontent.com/17688273/151362836-bc326fb1-0114-48bc-b21c-d244c1e08e33.png)

（10）[client -> server] setDataFrame()

![image](https://user-images.githubusercontent.com/17688273/151363724-c4d15e6c-bca7-48f5-942f-979df1edb011.png)

（11）[client -> server] Video Data; Audio Data

![image](https://user-images.githubusercontent.com/17688273/151363964-912042b6-167b-4343-b6bb-feb63b92ecf7.png)


## rtmp拉流player

### pull数据抓包分析

（1） tcp 3次 握手

（2）[client -> server] Handshake C0+C1 
![image](https://user-images.githubusercontent.com/17688273/151473525-429dc786-52db-42ef-9f29-861563f79e64.png)

（3）[server -> client] Handshake S0+S1+S2
![image](https://user-images.githubusercontent.com/17688273/151473719-61559bd7-9bcb-4a0a-80c4-2f77aa626a44.png)

（4）[client -> server] Handshake C2
![image](https://user-images.githubusercontent.com/17688273/151473808-244bb8f1-271f-461c-add9-0bc591d5e7ad.png)

（5）[client -> server] connect('live') 
![image](https://user-images.githubusercontent.com/17688273/151473936-116230a5-0e98-44ec-8725-2edebb095c40.png)

（6）[server -> client] Window Acknowledgement Size

（7）[client -> server] createStream()
![image](https://user-images.githubusercontent.com/17688273/151474326-69d445c4-de41-4b11-a380-1e063ec4c3c8.png)

（8）[server -> client] _result()
![image](https://user-images.githubusercontent.com/17688273/151474412-ba200052-2448-4e82-a2ed-dde50fb10a12.png)

（9）[client -> server] getStreamLength(); player('zhangbiwu')
![image](https://user-images.githubusercontent.com/17688273/151474561-5ba0db97-af49-4455-9e36-2179f6bfec18.png)

（10）[server -> client] Audio; Video
![image](https://user-images.githubusercontent.com/17688273/151474816-e83d5de2-fea8-4af8-ac49-3e3bb390f408.png)


# FAQ

## RTMP直播推流中需要注意的点
* 握手开始于客户端发送C0、C1块。 服务器收到C0或C1后发送S0和S1。
* 当客户端收齐S0和S1后，开始发送C2。 当服务器收齐C0和C1后，开始发送S2。
* 当客户端和服务器分别收到S2和C2后，握手完成。

# link

- [https://seminelee.com/2021/05/07/video-2/](https://seminelee.com/2021/05/07/video-2/)
