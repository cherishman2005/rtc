# RTP封装H.264码流

- [RTP分包模式](./RTP分包模式.md)


![image](https://github.com/user-attachments/assets/163c90f7-0eee-4048-9183-f7d7074bd366)

## 单NALU分组

此结构的NALU Header结构可以直接使用原始码流NALU Header，所以单NALU分组Type = 1~23。封装RTP包的时候可以直接把 查询到的NALU去掉起始码后的部分 当作单NALU分组的RTP包Payload部分。

![image](https://github.com/user-attachments/assets/54c8c70e-a56e-40d1-aa64-b62656998acc)

## 聚合分组STAP-A

通常采用STAP-A (Type=24)结构封装RTP聚合分组，下图为包含2个NALU的采用STAP-A结构的聚合分组。

![image](https://github.com/user-attachments/assets/1fb2abec-6265-4c09-ac08-2390743bf58d)

## 分片分组

通常采用无DON字段的FU-A结构封装RTP分片分组。各种RTP分组在RTP Header后面都跟着 F|NRI|Type 结构，来判定分组类型。

![image](https://github.com/user-attachments/assets/c79ff038-a235-4d10-a444-4fb1e5c2d241)

### FU indicator

采用FU-A分组类型的话Type = 28，NRI与此NALU中NRI字段相同。

![image](https://github.com/user-attachments/assets/9a38e274-c3b7-4ae7-a3bd-8227a22ac836)

### FU header

![image](https://github.com/user-attachments/assets/dd82c4e2-58fe-4776-bc60-bbc14d4d6ab6)

此结构中Type采用原始码流NALU中的Type字段，S=1表示这个RTP包为分片分组第一个分片，E=1表示为分片分组最后一个分片。除了首尾分片，中间的分片S&E都设为0。R为保留位，设为0。


# 参考链接

- [RTP协议](https://www.cnblogs.com/zhongqifeng/p/14789533.html)

