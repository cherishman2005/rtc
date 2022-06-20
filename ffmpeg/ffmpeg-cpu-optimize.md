# ffmpeg cpu optimize

## ffmpeg h264 encoder optimize

ffmpeg做h264编码，使用了libx264；想要降低cpu使用率，就需要牺牲图像质量；由于已经使用 baseline，就无需考虑B帧了。

目前能想到的，主要是需要调控以下一些参数：

1.qp值
此值范围为0~51 。值越小，量化步长越小，量化的精度就越高，意味着同样画质的情况下，产生的数据量可能会更大。数值越大，图像越模糊，CPU使用率越低。

2.设置编码速度preset
指定编码速度，速度越慢，画质越好，cpu占用越高，可取值：

ultrafast,superfast,veryfast,faster,fast,
medium,slow,slower,veryslow,placebo

调用方法：
x264_param_default_preset
3.partitions
H.264视频在压缩过程中划分为16x16的宏块。这些宏块可以进一步划分为更小的分割，这就是此选项要控制的部分。分割依不同帧类型（I、P、B）启用。可用的分割：p8x8, p4x4, b8x8, i8x8, i4x4, none, all

I：i8x8、i4x4。
P：p8x8（亦会启用p16x8/p8x16）、p4x4（亦会启用p8x4/p4x8）。
B：b8x8（亦会启用b16x8/b8x16）。
p4x4通常不怎么有用，而且性价比极低[4]。
4. no-cabac
预设值：b_cabac = 1。停用CABAC切换回效率较低的CAVLC。会降低压缩效率（通常10~20%）和解码的硬件需求。

coder：

熵编码类型，取值：

default
cavlc
cabac
vlc  
ac


## AV_CODEC_FLAG_LOW_DELAY

AV_CODEC_FLAG_LOW_DELAY该宏定义主要是应用于编码输出，降低输出的延时

通过ffplay RTSP H264解码走读，并没有发现用于解码低延时

通过测试例子设置AVCodecContext的flags:
 flags |= AV_CODEC_FLAG_LOW_DELAY

并没有达到实际的低延时解码的效果

 

在编码过程中减低延时，可以设置AV_CODEC_FLAG_LOW_DELAY标志位

针对特定的解码器mpeg4videodec，mpeg12dec等，可以实现解码低延时

# 参考链接

- [FFmpeg AV_CODEC_FLAG_LOW_DELAY剖析以及应用](https://blog.51cto.com/fengyuzaitu/3403507)
