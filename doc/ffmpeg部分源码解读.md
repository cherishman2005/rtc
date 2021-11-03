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
