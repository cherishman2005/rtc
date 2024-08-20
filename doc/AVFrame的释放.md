#【FFMPEG】AVFrame的释放

AVFrame申请，一般通过av_frame_alloc的方式申请；
但是，如果对AVFrame其中的data成员进行赋值，需要通过类似av_image_alloc的方式申请data的空间。

如果申请了data空间，需要再释放该AVFrame时，需要首先利用av_freep(AVFrame->data[0])，对其中的data空间进行释放；
然后使用av_frame_free对AVFrame进行释放。
