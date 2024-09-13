# ffmpeg

* AVFrame，AVPacket， pts， dts要控制好；

## 1路视频混画

* cpu消耗 120%，  预期是 40%  -- 待解决
  

## 2路直播视频混画cpu消耗

2路直播视频混画cpu消耗 300%

![image](https://github.com/user-attachments/assets/40de3689-ec6d-41b2-967f-612d209ddfcd)

* 优化后的效果

节约1个cpu

![image](https://github.com/user-attachments/assets/ec7814ed-adaa-4733-a0c7-c7d21aedbd64)

# FAQ

## 音视频处理（转码）很烧钱

![image](https://github.com/user-attachments/assets/0337b615-8467-40ce-82b6-6debeb5d3ef8)

转码16核都快用完了。

* 直播转码消耗的cpu资源好一些

![image](https://github.com/user-attachments/assets/68e75863-d9c6-4b7f-9dbf-04893b19930d)
