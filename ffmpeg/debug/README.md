# ffmpeg

## RTP: dropping old packet received too late

## max delay reached. need to consume packet

```
2024-10-21 09:32:32 [mix][warn][64232]max delay reached. need to consume packet
2024-10-21 09:32:32 [mix][warn][64232]RTP: missed 5108 packets
2024-10-21 09:32:32 [mix][warn][64232]max delay reached. need to consume packet
2024-10-21 09:32:32 [mix][warn][64232]RTP: missed 5109 packets
2024-10-21 09:32:32 [mix][warn][64232]max delay reached. need to consume packet
2024-10-21 09:32:32 [mix][warn][64232]RTP: missed 5110 packets
2024-10-21 09:32:32 [mix][warn][64232]max delay reached. need to consume packet
2024-10-21 09:32:32 [mix][warn][64232]RTP: missed 5111 packets
2024-10-21 09:32:32 [mix][warn][64232]max delay reached. need to consume packet
2024-10-21 09:32:32 [mix][warn][64232]RTP: missed 5112 packets
2024-10-21 09:32:32 [mix][warn][64232]max delay reached. need to consume packet
2024-10-21 09:32:32 [mix][warn][64232]RTP: missed 5113 packets
2024-10-21 09:32:32 [mix][warn][64232]max delay reached. need to consume packet
2024-10-21 09:32:32 [mix][warn][64232]RTP: missed 5114 packets
2024-10-21 09:32:32 [mix][warn][64232]max delay reached. need to consume packet
2024-10-21 09:32:32 [mix][warn][64232]RTP: missed 5115 packets
```

## attempted to set receive buffer to size 2048000 but it only ended up set as 425984

```
2024-10-21 13:08:28 [mix][warn][2895]attempted to set receive buffer to size 2048000 but it only ended up set as 425984
2024-10-21 13:08:28 [mix][warn][2895]attempted to set receive buffer to size 2048000 but it only ended up set as 425984
```

the latest ffmpeg2.8.5 already has this option. I use it to set the the buffer_size
```
av_dict_set(&options, "buffer_size", "655360", 0);
```
and I got this output:

[udp @ 0xb4945090] attempted to set receive buffer to size 655360 but it only ended up set as 327680 After some searching I run
```
echo 2097152 > /proc/sys/net/core/rmem_max
```
to fix the warning

# 参考链接

- [https://stackoverflow.com/questions/29075467/set-rtsp-udp-buffer-size-in-ffmpeg-libav](https://stackoverflow.com/questions/29075467/set-rtsp-udp-buffer-size-in-ffmpeg-libav)

