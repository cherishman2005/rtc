# ffmpeg

## ffmpeg性能压测

## 多个输入连接到单个输出文件

```
/usr/bin/ffmpeg -i vtest.mp4 -i vtest.mp4 -y -filter_complex concat=n=2:v=1:a=0 out.mp4
```

## ffmpeg多路转码
```
root      1522  1498 99 15:21 pts/1    00:04:08 /usr/bin/ffmpeg -i /data12/nodejs/video/win.HD.1080p.mp4 -y -acodec aac -strict experimental -vcodec libx264 -filter:v scale=w=640:h=400 -f mp4 /data12/nodejs/video/output-medium.mp4
root      1523  1498 99 15:21 pts/1    00:03:08 /usr/bin/ffmpeg -i /data12/nodejs/video/win.HD.1080p.mp4 -y -acodec aac -strict experimental -vcodec libx264 -filter:v scale=w=320:h=200 -f mp4 /data12/nodejs/video/output-small.mp4
root      1524  1498 99 15:21 pts/1    00:06:45 /usr/bin/ffmpeg -i /data12/nodejs/video/win.HD.1080p.mp4 -y -acodec aac -strict experimental -vcodec libx264 -f mp4 /data12/nodejs/video/output-original-size.mp4
```

ffmpeg软件转码太耗cpu
![image](https://user-images.githubusercontent.com/17688273/148635803-b4a17637-7445-4469-92b2-88d1bb600948.png)

## opencv+ai人脸检测

* opencv预处理
* AI-model process
* opencv后处理

![image](https://user-images.githubusercontent.com/17688273/148714857-163e7e23-bb23-4566-a1df-a4685d84c46f.png)

运行示例结果：

![image](https://user-images.githubusercontent.com/17688273/148715081-8cec06e5-e162-4fc6-bbc9-1d1c03a68e47.png)


# 参考链接

- [使用ffmpeg命令行实现一入多出转码](https://blog.csdn.net/xiaoluer/article/details/81346285)

- [opencv opencv4nodejs 安装和简单抠图](https://www.codenong.com/cs122064921/)

- [ffmpeg中的roi encoding介绍](https://blog.csdn.net/yjguo2004/article/details/104460763)
- [https://github.com/pogofdev/faceRecognitionDemo](https://github.com/pogofdev/faceRecognitionDemo)

- [https://github.com/DominicCabral/face-recognition](https://github.com/DominicCabral/face-recognition)
