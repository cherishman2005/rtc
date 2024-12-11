# ffmpeg-FAQ

## 调试rtmp收流

```
[NULL @ 0x2602040] non-existing PPS 0 referenced
[h264 @ 0x2602040] non-existing PPS 0 referenced
[h264 @ 0x2602040] decode_slice_header error
[h264 @ 0x2602040] no frame!
[h264 @ 0x2602040] non-existing PPS 0 referenced
    Last message repeated 1 times
[h264 @ 0x2602040] decode_slice_header error
[h264 @ 0x2602040] no frame!
[h264 @ 0x2602040] non-existing PPS 0 referenced
    Last message repeated 1 times
[h264 @ 0x2602040] decode_slice_header error
```

![image](https://github.com/user-attachments/assets/aff39ca8-0109-4d2d-9762-2f6817cbbe96)

### 分析思路

* tcpdump抓包分析
