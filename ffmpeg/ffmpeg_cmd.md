# ffmpeg comand

ffmpeg rtp
```
ffmpeg -loglevel debug -fflags +genpts -f sdp -i pipe:0 -map 0:v:0 -c:v copy -flags +global_header ./files/1648302225797.webm
```

ffmpeg抽帧截图
```
ffmpeg -i https://**.bj.bcebos.com/dataset/zhangbiwu/video/test.mp4 -y -f image2 -vframes 1 -vcodec png -f rawvideo -s 320x240 -ss 00:00:05 ./files/snapshot-01.png
```

# 小结

## webrtc录制
100M    1648647242737.webm  -- webrtc录制了12小时，只需100M
