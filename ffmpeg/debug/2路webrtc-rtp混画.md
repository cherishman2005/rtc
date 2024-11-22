# 2路webrtc-rtp混画

* 2路webrtc rtp混画，并转推rtmp直播

出现花屏

![image](https://github.com/user-attachments/assets/c5398423-d574-431f-8420-c3f6de926036)

![image](https://github.com/user-attachments/assets/e694e7d8-1131-4264-819a-0b91f7266df5)


## 分析

花屏的帧率fps=30，可以适当降低帧率

![image](https://github.com/user-attachments/assets/a65bb04f-bb96-4559-a2ff-0b837846cf9b)


## 优化

### 降低帧率

帧率调整为fps=15

![image](https://github.com/user-attachments/assets/3f737d91-5dbb-4f0c-abdd-03869048870b)

降低帧率后，性能好了很多，减少了花屏现象

![image](https://github.com/user-attachments/assets/836278e9-d472-499c-8e8c-ed8dccd0b7ab)

### 降低码率

### 优化效果

逐步优化，现在清晰了很多

![d8a25a1007e70feb971e92e66b5fa18](https://github.com/user-attachments/assets/625fecfc-2a82-4226-b7cd-d9a403eb2854)


![image](https://github.com/user-attachments/assets/ee6ab7e0-fee5-42ae-a459-beb22cd3f7fd)

