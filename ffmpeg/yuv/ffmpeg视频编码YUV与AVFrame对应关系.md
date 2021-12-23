# ffmpeg视频编码YUV与AVFrame对应关系

最近群里有人问：NV12格式，怎么对应AVFrame中的data[0]，data[1]，data[2]。其实ffmpeg视频编码，YUV与AVFrame对应关系很简单。

在视频编码时，我们需要把YUV数据拷贝到AVFrame.data中，视频编码有硬件加速以及非硬件加速两种，所以对应关系也有两种。

## 硬件加速编码
指通过显卡进行硬件加速编码，例如指定vaapi进行编码。使用硬件加速编码时，YUV输入格式一般都是NV12，我做过的Intel以及Nvidia编码都是这样。之所以使用NV12格式，在Intel开发文档有这样说明：

Based on the NV12 color format: Decode/encode and VPP operations use NV12 because this
provides better performance than other formats such as YUV. While it is possible to include color
conversion filters it is best if pipelines can be arranged to minimize conversion steps by doing as
much consecutive work in NV12 as possible.

因为ffmpeg底层也是调用相关显卡SDK做硬件加速编码，所以我们把YUV数据拷贝给AVFrame时，也得按NV12格式。对于NV12格式，Y数据在最前，接着是UV交错排布，类似这样：YYYYYYYY UVUV，所以对于AVFrame，我们得把Y数据拷贝data[0]，UV数据拷贝给data[1]。
```
AVFrame.data[0] : YYYYYYYY
AVFrame.data[1] : UVUV
```

## 非硬件加速编码
指通过CPU进行编码，例如指定libx264，libx265进行编码。对应关系就比较简单了，YUV三个分量数据依次对应data[0]，data[1]，data[2]。
```
AVFrame.data[0] : YYYYYYYY
AVFrame.data[1] : UU
AVFrame.data[2] : VV
```
