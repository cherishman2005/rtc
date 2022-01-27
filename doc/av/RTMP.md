# RTMP

## rtmp推流

### push数据抓包分析

（1）tcp3次握手

![image](https://user-images.githubusercontent.com/17688273/151356256-db02b030-0b67-40e5-8f1b-e0083a10ac19.png)


（2）Handshake C0+C1 (client -> server)

![image](https://user-images.githubusercontent.com/17688273/151356966-b274e0bb-cfcb-44f3-8d05-a46aea46d271.png)


（3）Handshake S0+S1+S2 (server -> client)

![image](https://user-images.githubusercontent.com/17688273/151357271-78302be9-87f6-46c3-8065-23f252dc3501.png)

（4）Handshake C2 (client -> server)

![image](https://user-images.githubusercontent.com/17688273/151357896-8da6a4ef-d340-4050-8613-b3ee06fa7eb5.png)


（5）connect('live') (client -> server)

![image](https://user-images.githubusercontent.com/17688273/151359231-20b2f87a-cd5c-4986-ad9b-57c27ce5d1a5.png)


（6）Window Acknowledgement Size(server -> client)

![image](https://user-images.githubusercontent.com/17688273/151360346-275a8dfc-810e-4ae5-92a2-02fa6318ff3f.png)
