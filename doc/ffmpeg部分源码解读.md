# ffmpeg部分源码解读

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
