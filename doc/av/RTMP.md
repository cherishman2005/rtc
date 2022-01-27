# RTMP

## rtmp推流

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

# FAQ

## RTMP直播推流中需要注意的点
* 握手开始于客户端发送C0、C1块。 服务器收到C0或C1后发送S0和S1。
* 当客户端收齐S0和S1后，开始发送C2。 当服务器收齐C0和C1后，开始发送S2。
* 当客户端和服务器分别收到S2和C2后，握手完成。
