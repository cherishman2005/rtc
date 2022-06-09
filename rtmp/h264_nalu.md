# H264码流解析及NALU

```
//H264定义的类型 values for nal_unit_type

typedef enum {

 NALU_TYPE_SLICE    = 1,

 NALU_TYPE_DPA      = 2,

 NALU_TYPE_DPB      = 3,

 NALU_TYPE_DPC      = 4,

 NALU_TYPE_IDR      = 5,

 NALU_TYPE_SEI      = 6,

 NALU_TYPE_SPS      = 7,

 NALU_TYPE_PPS      = 8,

 NALU_TYPE_AUD      = 9,

 NALU_TYPE_EOSEQ    = 10,

 NALU_TYPE_EOSTREAM = 11,

 NALU_TYPE_FILL     = 12,

#if (MVC_EXTENSION_ENABLE)

 NALU_TYPE_PREFIX   = 14,

 NALU_TYPE_SUB_SPS  = 15,

 NALU_TYPE_SLC_EXT  = 20,

 NALU_TYPE_VDRD     = 24  // View and Dependency Representation Delimiter NAL Unit

#endif

} NaluType;
```


## AVCDecoderConfigurationRecord

当 AVC packet 类型的类型为 0 是，则表示是解码器配置，即 sps，pps，保存控制信息。此时数据区的格式如下图：
image
记录sps，pps信息。一般出现在第二个tag中，紧跟在onMeta之后。

一个典型的序列：
```
0000190: 0900 0033 0000 0000 0000 0017 0000 0000  ...3............

00001a0: 0164 002a ffe1 001e 6764 002a acd9 4078  .d.*....gd.*..@x

00001b0: 0227 e5ff c389 4388 0400 0003 0028 0000  .'....C......(..

00001c0: 0978 3c60 c658 0100 0568 ebec b22c 0000  .x<`.X...h...,..
```

17: 表示h264IDR data
00：表示是AVC序列头
00 00 00 ：cts为0
//从此往下就是AVCDecoderConfigurationRecord

01 ：版本号
64 00 2a：profile level id，sps的三个字节，64 表示是h264 high profile，2a表示level。
FF：NALU长度，为3？不知道这个长度用在哪里。
E1：表示下面紧跟SPS有一个。
//sps[N]：sps数组。

00 1e: 前面是两个字节的sps长度，表示后面的sps的长度是1e大小。
6764 002a acd9 4078 0227 e5ff c389 4388 0400 0003 0028 0000 0978 3c60 c658：sps的数据。
//因为只有一个sps，跳过这些长度，然后就是pps的个数信息：

01 ：pps个数，1
//pps[n] pps 的个数

00 05：表示pps的大小是5个字节。
68 eb ec b2 2c：pps的数据
00 00 …….这是下一个tag 的内容了
如下图，为通过 obs 推 FLV 文件到 RTMP 服务器抓包得到的 video sps，pps 的包：
![image](https://user-images.githubusercontent.com/17688273/172858897-06112a5d-1a63-4015-81bd-ec3b0062ae14.png)

# 参考链接

- [H264码流解析及NALU](https://www.cnblogs.com/jingzhishen/p/3965868.html)
- [多媒体文件格式之FLV](https://www.cnblogs.com/jimodetiantang/p/8992425.html)
