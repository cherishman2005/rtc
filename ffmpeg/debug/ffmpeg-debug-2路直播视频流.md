# ffmpeg 2路直播视频流


## ffmpeg-debug-2路视频画中画

画中画的图片 出现闪烁： -- 待解决

在很多直播分享的场景 也会遇到这种情况。（如主画面是PPT，分享人在小窗口）

![image](https://github.com/user-attachments/assets/8280231a-50fe-4e63-b708-9c501cf1be52)


## ffmpeg-debug-2路视频上下分布

* 问题： 下半视频时而正常，时而花屏。

![image](https://github.com/user-attachments/assets/b68951ea-4201-4188-a212-bf08b0687345)

![image](https://github.com/user-attachments/assets/ac8dae4b-0fb2-425d-8bba-b38cedeb8b82)



* 如果是相同的流混流，不会有异常 -- 说明还是同步没有处理好

![image](https://github.com/user-attachments/assets/158a2ebd-fcce-48fa-93ca-99811a34dc5f)


## ffmpeg-debug-2路视频左右分布

![Uploading image.png…]()

