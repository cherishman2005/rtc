# coturn

## turn协议的工作原理

### Allocate请求

客户端通过发送Allocate请求给STUN服务器，从而让STUN服务器为A用户开启一个relay端口。

![image](https://github.com/cherishman2005/rtc/assets/17688273/0e0d238d-d4e4-4f37-b95f-ff99648edc6d)

a) 客户端A向STUN Port发送Allocate请求(图中绿色部分)

b) STUN服务器接收到客户端A的Allocate请求，服务器一看是Allocate请求，则根据relay端口分配策略为A分配一个端口。

c) 服务器发送response成功响应。在该response中包含XOR-RELAYED-ADDRESS属性。该属性值就是A的relay端口的异或结果。

d) 客户端接收到response后，就知道了自己的relay地址。该relay地址是个公网地址，可以看作是客户端A在公网上的一个代理，任何想要联系A的客户 端，只要将数据发送到A的relay地址就可以了，具体的转发原理请看下一小节。

### Relay端口消息的转发

任何想要联系客户端A的人，只要知道客户端A的relay地址就可以了。

（1）A的Relay端口接受其他客户端的消息

![image](https://github.com/cherishman2005/rtc/assets/17688273/8c41520e-10e1-44ce-b86d-8588903af388)

如上图所示：因为客户端A位于NAT后，所以其他客户端无法和A建立直接的通信。但是客户端A在STUN服务器上申请了一个端口(上图中：A的relay端口)，其他客户端想要和A通信，那么只需要将信息发送到“A的relay端口”，STUN服务器会将从relay端口接收到的信息通过STUN Port发送给A。

（2）A的响应消息原路返回
A应答其他客户端发来的消息的时候，是通过原路返回的。

![image](https://github.com/cherishman2005/rtc/assets/17688273/c79dfd9c-70c2-4ccb-bec5-c3b78dfe40ff)


**思考**

1.STUN服务器为什么不直接从A的relay端口把数据转发给A呢（如下图所示）？而非要从STUN端口发送？

![image](https://github.com/cherishman2005/rtc/assets/17688273/336d3a7d-b742-4084-9bfb-0e2208899f6f)


2.客户端A的响应消息在原路返回的时候，A的响应消息是先发送到了STUN Port，然后再经由A的relay Port发出的。那么A的relay Port是怎么知道它要把数据发送到哪呢？

### Refresh请求

STUN服务器给客户端A分配的relay地址都具有一定的有效时长，可能是30秒或者1分钟或者几十分钟。客户端如果需要STUN服务器一直为它开启这个端口，就需要定时的向STUN服务器发送请求，该请求用刷新relay端口的剩余时间。
在标准的TURN（RFC 5766）协议中，客户端A向STUN服务器发送Allocate请求，STUN服务器在响应消息中添加了一个“LifeTime”的属性，该属性表示relay的存活时间。 客户端需要在relay的存活时间内周期性的调用REFRESH请求，服务端接收到REFRESH请求后，刷新剩余时间；当REFRESH请求中的lifetime属性为0时，说明是客户端主动要求关闭relay地址。

### STUN端口的保活

由于与STUN服务器通信使用的是UDP，所以为了保持一个长连接，需要客户端周期性的向STUN服务器的STUN Port发送心跳包。
周期性心跳包的目的就是，使得NAT设备对客户端A的反射地址(Server Reflexive Address)一直有效。使得从STUN Port发送的数据能通过A的反射地址到达A。此处不理解的可以查阅“NAT 类型的分类以及NAT的作用”。
此处解释了，7.2.2.3中的第一个问题，因为客户端A没有和relay Port保活，又由于NAT的特性，数据直接通过relay port转发给A时，NAT直接就丢弃了，所以A是收不到的。所以数据必须经过STUN服务器的STUN Port发送。

### Relay转发的时候添加STUN头（Send和Data请求）

![image](https://github.com/cherishman2005/rtc/assets/17688273/1f09ceca-6136-494d-97dc-84e51e22dcca)

如上图所示是B主动给A发消息：“Hello”，A回应“Hi”的过程。

序号1、2、3、4、5为B的发送请求(蓝色箭头方向)；

序号6、7、8、9、10为A的回应，原路返回（绿色箭头方向）。

注意：在“Hello”发送的过程中，1、2阶段时，发送的数据为裸的UDP数据。在4、5过程中，是被STUN协议包装过的“Hello”，称之为Data indication。

同样在“Hi”发送的过程中，6、7阶段为被STUN协议包装过的“Hi”，称之为Send indication，9、10是裸的UDP数据。

在4、5阶段，由于数据是从STUN Port转发下来的，为了能够让客户端A知道这个包是哪个客户端发来的，所以，STUN 协议对“Hello”进行了重新的包装，最主要的就是添加了一个XOR-PEER-ADDRESS属性，由裸数据包装成STUN协议的过程，我们称之为添加了STUN头。XOR-PEER-ADDRESS的内容就是客户端B的反射地址（Server Reflexive Address）。

在6、7阶段，A的响应原路返回，为了能够让A的relay port知道最终发往哪个客户端，因此也为“Hi”添加了STUN头，也是添加了XOR-PEER-ADDRESS属性，内容就是客户端B的反射地址（Server Reflexive Address）。这样A的relay port就知道“Hi”的目的地址。

第3阶段是：从A的relay端口收到数据，添加STUN头后，最后从STUN Port 发出的过程。

第8阶段是：从STUN Port 接收到带STUN 头的数据，去掉STUN头，最后从A的relay端口发出的过程。

客户端B主动发送信息给A的交互流程如上图所示，那么客户端A主动发送信息给客户端B的交互流程是怎样的呢，你能画出来吗？

要知道客户端A主动发消息给客户端B，应该将消息发往客户端B的relay port。
