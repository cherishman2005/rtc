# ffmpeg部分源码解读

## 最常用基础api

### av_init_packet
```
void av_init_packet(AVPacket *pkt)
{
    pkt->pts                  = AV_NOPTS_VALUE;
    pkt->dts                  = AV_NOPTS_VALUE;
    pkt->pos                  = -1;
    pkt->duration             = 0;
#if FF_API_CONVERGENCE_DURATION
FF_DISABLE_DEPRECATION_WARNINGS
    pkt->convergence_duration = 0;
FF_ENABLE_DEPRECATION_WARNINGS
#endif
    pkt->flags                = 0;
    pkt->stream_index         = 0;
    pkt->buf                  = NULL;
    pkt->side_data            = NULL;
    pkt->side_data_elems      = 0;
}
```

### av_new_packet

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

### av_free_packet

```
#if FF_API_AVPACKET_OLD_API
FF_DISABLE_DEPRECATION_WARNINGS
void av_free_packet(AVPacket *pkt)
{
    if (pkt) {
        if (pkt->buf)
            av_buffer_unref(&pkt->buf);
        pkt->data            = NULL;
        pkt->size            = 0;

        av_packet_free_side_data(pkt);
    }
}
FF_ENABLE_DEPRECATION_WARNINGS
#endif
```

### av_packet_free

```
void av_packet_free(AVPacket **pkt)
{
    if (!pkt || !*pkt)
        return;

    av_packet_unref(*pkt);
    av_freep(pkt);
}
```

### av_frame_free

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

### avcodec_free_context

释放编码器/解码器上下文
```
void avcodec_free_context(AVCodecContext **pavctx)
{
    AVCodecContext *avctx = *pavctx;

    if (!avctx)
        return;

    avcodec_close(avctx);

    av_freep(&avctx->extradata);
    av_freep(&avctx->subtitle_header);
    av_freep(&avctx->intra_matrix);
    av_freep(&avctx->inter_matrix);
    av_freep(&avctx->rc_override);

    av_freep(pavctx);
}
```

## 视频解码

```
// This does not quite work like avcodec_decode_audio4/avcodec_decode_video2.
// There is the following difference: if you got a frame, you must call
// it again with pkt=NULL. pkt==NULL is treated differently from pkt->size==0
// (pkt==NULL means get more output, pkt->size==0 is a flush/drain packet)
static int decode(AVCodecContext *avctx, AVFrame *frame, int *got_frame, AVPacket *pkt)
{
    int ret;

    *got_frame = 0;

    if (pkt) {
        ret = avcodec_send_packet(avctx, pkt);
        // In particular, we don't expect AVERROR(EAGAIN), because we read all
        // decoded frames with avcodec_receive_frame() until done.
        if (ret < 0 && ret != AVERROR_EOF)
            return ret;
    }

    ret = avcodec_receive_frame(avctx, frame);
    if (ret < 0 && ret != AVERROR(EAGAIN))
        return ret;
    if (ret >= 0)
        *got_frame = 1;

    return 0;
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

# 附注

![image](https://user-images.githubusercontent.com/17688273/143805203-259b6424-049d-4c13-9e36-7340c5d5dbd1.png)


# 参考链接

- [FFmpeg中的硬件加速编码器](https://meta.appinn.net/t/topic/18299)
- [c++ - 使用C ++中的ffmpeg hwaccel](https://mlog.club/article/178278)
