# websocket开发技巧

* 通过http握手，然后upgrade升级为websocket长连接；

* 一般通过nginx ssl代理，将ws升级为wss；

  ** 架构简单时，采用nginx upstream；

  ** 如果系统容量大时，可将后端服注册到微服务中心，nginx去拉取ip+port列表。非常灵活方便。

## websocket协议字段

```
      0                   1                   2                   3
      0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
     +-+-+-+-+-------+-+-------------+-------------------------------+
     |F|R|R|R| opcode|M| Payload len |    Extended payload length    |
     |I|S|S|S|  (4)  |A|     (7)     |             (16/64)           |
     |N|V|V|V|       |S|             |   (if payload len==126/127)   |
     | |1|2|3|       |K|             |                               |
     +-+-+-+-+-------+-+-------------+ - - - - - - - - - - - - - - - +
     |     Extended payload length continued, if payload len == 127  |
     + - - - - - - - - - - - - - - - +-------------------------------+
     |                               |Masking-key, if MASK set to 1  |
     +-------------------------------+-------------------------------+
     | Masking-key (continued)       |          Payload Data         |
     +-------------------------------- - - - - - - - - - - - - - - - +
     :                     Payload Data continued ...                :
     + - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - +
     |                     Payload Data continued ...                |
     +---------------------------------------------------------------+
```

* `websocket实质是私有协议，只是被统一为了标准协议`

* 应用协议采用FIN字段进行分片；—— 开发后端时，重点注意对这个字段的处理；

## websocket开发场景

* IM/chatroom即时通信系统；

* websocket在webrtc做控制信令；

* 新的长连接协议一般采用websocket，特别是在IM聊天系统中；

  ** 开发成本低：不需要维护TCP长连接sdk；在websocket API基础上开发应用层协议。
  
  ** 互通成本低：移动端、web端、小程序端均采用websocket长连接。
  
  ** websocket API接口简单。

* wss-flv websocket传输媒体协议flv；
  
## websocket开发的关键

  * 【重点】链接状态管理机制；（disconnect, conencting, connected等状态跃迁）
  
  * 面向对象的方法管理房间（群）； —— 千万不要采用结构化的方式设计；

  * login API登录超时机制优化；—— 在socket通道还没ready时，应该做login请求先缓存；
  
  * login/logout、join/leave关键API的采用互斥锁进行串行约束；（后面博客会分享）；
  
  * 小心setTimeout/setInterval使用的各种坑；
  
  * token管理机制； —— `优化设计时要多研究分布式实时系统的定时同步机制`

  * 应用层协议设计：不要太复杂。最好可采用json或protobuf。
  
   【注】如果是自定义序列化、反序列化，而没有对应的成熟序列化工具，需要手写序列化、反序列； —— 开发效率非常低；所以尽量采用通用简单的序列化方式。
  
# 参考链接

* https://tools.ietf.org/html/rfc6455
