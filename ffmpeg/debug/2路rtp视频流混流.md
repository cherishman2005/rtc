# 2路rtp视频流混流

2路rtp视频流进行混流，并rtmp转推到CDN

![image](https://github.com/user-attachments/assets/d19ebdb1-dcce-466a-a2ad-fe1aea896858)

【问题描述】中途出现花屏，卡顿；播放 不流畅

![image](https://github.com/user-attachments/assets/b4b24a0f-dd56-40f0-b070-bbc6ab787051)

```
[sdp @ 0x45f6680] max delay reached. need to consume packet
[sdp @ 0x45f6680] RTP: missed 28 packets
[h264 @ 0x45f9c00] cbp too large (3199971767) at 23 10
[h264 @ 0x45f9c00] error while decoding MB 23 10
[h264 @ 0x45f9c00] concealing 282 DC, 282 AC, 282 MV errors in P frame
```
