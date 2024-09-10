# ffmpeg 2路直播视频流


## ffmpeg-debug-2路视频画中画

画中画的图片 出现闪烁。

在很多直播分享的场景 也会遇到这种情况。（如主画面是PPT，分享人在小窗口）

![image](https://github.com/user-attachments/assets/8280231a-50fe-4e63-b708-9c501cf1be52)

* 【原因】帧同步没有处理好，已解决

![image](https://github.com/user-attachments/assets/992da2e0-c8fb-45c8-9bbe-b30635f8cb23)


## ffmpeg-debug-2路视频上下分布

* 问题： 下半视频时而正常，时而花屏。

![image](https://github.com/user-attachments/assets/b68951ea-4201-4188-a212-bf08b0687345)

![image](https://github.com/user-attachments/assets/ac8dae4b-0fb2-425d-8bba-b38cedeb8b82)



* 如果是相同的流混流，不会有异常 -- 说明还是同步没有处理好

![image](https://github.com/user-attachments/assets/158a2ebd-fcce-48fa-93ca-99811a34dc5f)


### 【原因】帧同步没有处理好，已解决

![image](https://github.com/user-attachments/assets/fd1cee49-1954-4ae4-92d1-d6684ea207b8)

### 1路视频结束后对应的画面一直黑屏，怎么优化？

![image](https://github.com/user-attachments/assets/36362ad0-ab64-427a-84a7-fdd904fbe91f)

没有问题，因为最后的视频流，最后一帧就是黑屏。-- 右下角有个水印。

### 上下分布时，下半部分有瑕疵--黑边

![image](https://github.com/user-attachments/assets/a400d157-75a8-4834-b7bd-43e993098110)


## ffmpeg-debug-2路视频左右分布

![image](https://github.com/user-attachments/assets/50c00656-e404-4130-bed4-dc8273fc4488)



# FAQ

## 模拟直播推流的片源会不会有问题？

* 通过ffmpeg对片源重新编码，解决片源问题
```
./ffmpeg -i test.mp4  -c:v libx264 -c:a aac -f mp4 test1.mp4
```

## 音视频目前业界做的很好了，为什么还有做呢？

* 我尽量做到成本最低。
  * 很多是1个大team在做，并且是私有协议； -- 客户端，服务端音视频处理，传输 都是不同的团队，演进升级需要各方配合，人力成本较高。
  * 尽量适配标准协议；-- 方便与其他组件（客户端、播放器）对接；

* 尽量针对几个技术点，做到极致优化；

* 充分发挥成本优势 和 关键技术竞争力。

* 如果做的足够好时，可以尝试着开源出去，让更多的人加入进来。

## 2路直播流，混流时 如果这2路直播流的宽高参数不相同时，怎么混流？

* 对视频进行缩放

* 建议： 尽量让混流的2路直播流宽高相同，缩放操作需要消耗更多的cpu资源。


