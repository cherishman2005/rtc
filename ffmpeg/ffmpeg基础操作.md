![image](https://github.com/cherishman2005/rtc/assets/17688273/f75b631b-f87d-43d2-84f9-8f7e04605402)# ffmpeg基础操作

## ffmpeg后端处理添加水印

1. watermark

![image](https://github.com/cherishman2005/rtc/assets/17688273/b873a764-d288-4072-898b-b327f8fc6202)

2. 通过水印实现画中画效果

![image](https://github.com/cherishman2005/rtc/assets/17688273/31720a79-455b-423d-b0d7-28e585b1f841)
https://github.com/cherishman2005/rtc/blob/master/ffmpeg/ffmpeg%E5%9F%BA%E7%A1%80%E6%93%8D%E4%BD%9C.md
3. ffmpeg万能工具，画中画直播推流： 1路是webrtc直播推流，1路是点播视频，实现画中画。
![image](https://github.com/cherishman2005/rtc/assets/17688273/03ce530d-a2e3-4d20-b223-024605e8e1f1)

4. ffmpeg 2路rtp直播流，然后混流，实现画中画

![image](https://github.com/cherishman2005/rtc/assets/17688273/d2530ac0-79e1-480e-b12f-354f33df9ac3)

5. 2路混流，左右分布

![image](https://github.com/cherishman2005/rtc/assets/17688273/eebb9541-a728-41e2-abc0-3e2acd7da720)


6. 2路混流，上下分布

![image](https://github.com/cherishman2005/rtc/assets/17688273/5e177c13-72d8-4c82-9068-d7a8c4f4c180)

# FAQ

## 2路混画在播放过程中出现花屏

![a6ec105ee4e0ac775d044739017fb8c](https://github.com/cherishman2005/rtc/assets/17688273/cdbf75dd-6225-4022-a9bd-fb1f7b06b30b)

这种混画，出现花屏 有没有定位思路， 或解决方法？
