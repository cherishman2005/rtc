# mjpeg图片编码

```
AVCodec ff_mjpeg_encoder = {
    .name           = "mjpeg",
    .long_name      = NULL_IF_CONFIG_SMALL("MJPEG (Motion JPEG)"),
    .type           = AVMEDIA_TYPE_VIDEO,
    .id             = AV_CODEC_ID_MJPEG,
    .priv_data_size = sizeof(MpegEncContext),
    .init           = ff_mpv_encode_init,
    .encode2        = ff_mpv_encode_picture,
    .close          = ff_mpv_encode_end,
    .capabilities   = AV_CODEC_CAP_SLICE_THREADS | AV_CODEC_CAP_FRAME_THREADS,
    .caps_internal  = FF_CODEC_CAP_INIT_THREADSAFE | FF_CODEC_CAP_INIT_CLEANUP,
    .pix_fmts       = (const enum AVPixelFormat[]) {
        AV_PIX_FMT_YUVJ420P, AV_PIX_FMT_YUVJ422P, AV_PIX_FMT_YUVJ444P, AV_PIX_FMT_NONE
    },
    .priv_class     = &mjpeg_class,
    .profiles       = NULL_IF_CONFIG_SMALL(ff_mjpeg_profiles),
};
```
