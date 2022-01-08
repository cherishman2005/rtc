# ffmpeg

## ffmpeg性能压测


## ffmpeg多路转码
```
root      1522  1498 99 15:21 pts/1    00:04:08 /usr/bin/ffmpeg -i /data12/nodejs/video/win.HD.1080p.mp4 -y -acodec aac -strict experimental -vcodec libx264 -filter:v scale=w=640:h=400 -f mp4 /data12/nodejs/video/output-medium.mp4
root      1523  1498 99 15:21 pts/1    00:03:08 /usr/bin/ffmpeg -i /data12/nodejs/video/win.HD.1080p.mp4 -y -acodec aac -strict experimental -vcodec libx264 -filter:v scale=w=320:h=200 -f mp4 /data12/nodejs/video/output-small.mp4
root      1524  1498 99 15:21 pts/1    00:06:45 /usr/bin/ffmpeg -i /data12/nodejs/video/win.HD.1080p.mp4 -y -acodec aac -strict experimental -vcodec libx264 -f mp4 /data12/nodejs/video/output-original-size.mp4
```

ffmpeg软件转码太耗cpu
![image](https://user-images.githubusercontent.com/17688273/148635803-b4a17637-7445-4469-92b2-88d1bb600948.png)
