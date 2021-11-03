# ffmpeg部分源码解读

## av_new_packet

```
int av_new_packet(AVPacket *pkt, int size)
{
    AVBufferRef *buf = NULL;
    int ret = packet_alloc(&buf, size);
    if (ret < 0)
        return ret;

    get_packet_defaults(pkt);
    pkt->buf      = buf;
    pkt->data     = buf->data;
    pkt->size     = size;

    return 0;
}
```

## av_frame_free

av_frame_free做了入参为空（nullptr）的异常保护：
```
void av_frame_free(AVFrame **frame)
{
    if (!frame || !*frame)
        return;

    av_frame_unref(*frame);
    av_freep(frame);
}
```

## swscale

图像拉伸
FFmpeg支持多种像素拉伸的方式。这些方式的定义位于libswscale\swscale.h中，如下所示。
```
#define SWS_FAST_BILINEAR     1
#define SWS_BILINEAR          2
#define SWS_BICUBIC           4
#define SWS_X                 8
#define SWS_POINT          0x10
#define SWS_AREA           0x20
#define SWS_BICUBLIN       0x40
#define SWS_GAUSS          0x80
#define SWS_SINC          0x100
#define SWS_LANCZOS       0x200
#define SWS_SPLINE        0x400
```
其中SWS_BICUBIC性能比较好；SWS_FAST_BILINEAR在性能和速度之间有一个比好好的平衡，
而SWS_POINT的效果比较差。

## cuda

```
AVHWAccel ff_mjpeg_nvdec_hwaccel = {
    .name                 = "mjpeg_nvdec",
    .type                 = AVMEDIA_TYPE_VIDEO,
    .id                   = AV_CODEC_ID_MJPEG,
    .pix_fmt              = AV_PIX_FMT_CUDA,
    .start_frame          = nvdec_mjpeg_start_frame,
    .end_frame            = ff_nvdec_simple_end_frame,
    .decode_slice         = nvdec_mjpeg_decode_slice,
    .frame_params         = nvdec_mjpeg_frame_params,
    .init                 = ff_nvdec_decode_init,
    .uninit               = ff_nvdec_decode_uninit,
    .priv_data_size       = sizeof(NVDECContext),
};
```

nvidia不支持mjpeg硬件编解码：

![image](https://user-images.githubusercontent.com/17688273/139840920-ef18ec1f-4fa3-458c-8696-1dab9b9d6e90.png)

# 参考链接

- [FFmpeg中的硬件加速编码器](https://meta.appinn.net/t/topic/18299)
- [c++ - 使用C ++中的ffmpeg hwaccel](https://mlog.club/article/178278)
