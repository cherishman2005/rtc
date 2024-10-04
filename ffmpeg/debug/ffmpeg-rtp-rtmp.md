# ffmpeg-rtp-rtmp

## 分辨率跳变
ffmpeg接收rtp流，转推rtmp流时， 运行一段时间后分辨率跳变。

![image](https://github.com/user-attachments/assets/368da15b-759c-4a16-9fd9-81059c6483c2)


```
[libx264 @ 0x3297a00] Input picture width (1280) is greater than stride (1024)
do_video_out avcodec_send_frame in_picture:0x3ab6000, video_size:960x540, enc:1280x720, Error occurred: -542398533:Generic error in an external library
```

![image](https://github.com/user-attachments/assets/97147a01-92b5-4c17-9cb9-c913ae5fc32c)

AVFrame视频帧分辨率与目标分辨率不一致时怎么 处理？


### 解决方法

* 有不同的解决方案：
  （I）从推流端角度，不让分辨率跳变；
  （II）混流时做适配。
已解决

## rtp混流1分钟后断流

* 控制信令进行优化。已解决

## webrtc -> rtmp直播偶尔模糊

webrtc -> rtmp直播中途出现模糊，然后再恢复。

![image](https://github.com/user-attachments/assets/71b2dad1-7dc4-43b5-872f-2a675dd8e6be)

