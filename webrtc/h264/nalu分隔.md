# H.264 NALU分隔Annex B和avcC

## 分隔格式

H.264常用的分隔方式有Annex B和avcC

### Annex B

这种分隔符通常用于视频会议还有文件存储例如TS等

用VLC打开avcC格式的视频文件，编码信息中显示H264 - MPEG-4 AVC（part 10）(h264)

Annex B的格式如下，start code有可能是{0 0 0 1}或者{0 0 1}，{0 0 0 1}通常用于第一个NALU、SPS和PPS，其他地方使用{0 0 1}以减少内存占用
```
([start code] NALU) | ( [start code] NALU) | ...
```

### avcC

这种分隔符通常用于文件存储例如mp4、flv，还有直播rtmp等

用VLC打开avcC格式的视频文件，编码信息中显示H264 - MPEG-4 AVC（part 10）(avc1)

avcC的格式如下，字段length所占的字节长度由extradata中的NALULengthSizeMinusOne字段决定，length的值由NALU的实际长度决定

```
([extradata]) | ([length] NALU) | ([length] NALU) | ...
```
