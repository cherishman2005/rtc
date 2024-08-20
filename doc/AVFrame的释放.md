# 【FFMPEG】AVFrame的释放

AVFrame申请，一般通过av_frame_alloc的方式申请；
但是，如果对AVFrame其中的data成员进行赋值，需要通过类似av_image_alloc的方式申请data的空间。

如果申请了data空间，需要再释放该AVFrame时，需要首先利用av_freep(AVFrame->data[0])，对其中的data空间进行释放；
然后使用av_frame_free对AVFrame进行释放。

## av_frame_free和av_frame_unref区别

av_frame_unref函数会将AVFrame中的数据缓冲区引用计数减一，如果引用计数为0，会自动释放数据缓冲区的内存。 而av_frame_free函数则会释放AVFrame本身的内存，因此在释放AVFrame时需要先确保数据缓冲区的内存已经被正确释放。

## 使用ffmpeg解码 需要注意的内存泄漏问题

说说项目中遇到的两点失误：

1. AVFrame结构，av_frame_alloc申请内存，av_frame_free释放内存。容易混淆的是av_frame_unref，它的作用是释放音视频数据资源，而av_frame_free是释放所有资源，包括音视频数据资源和结构体本身的内存。可以从源码中看到，av_frame_free函数体内是先调用了av_frame_unref释放数据缓存，再free本身结构体内存。

2. AVPacket结构，比较神奇的是，av_read_frame执行过程中，内部为形参pkt自动申请一块缓存用来存储音视频数据，而释放资源却由我们自己调用av_free_packet来释放缓存。从源码分析得知，当av_read_frame返回值不小于0时，内部会进行缓存操作，需要外界释放；当返回值小于0时，内部会进行缓存并释放，或者不进行缓存，由数据本身损坏或正常结束来决定。

相关源码可以参看：

https://github.com/FFmpeg/FFmpeg/blob/master/libavutil/frame.c

https://www.ffmpeg.org/doxygen/0.6/avpacket_8c-source.html

https://ffmpeg.org/doxygen/trunk/libavformat_2utils_8c-source.html

https://www.ffmpeg.org/doxygen/2.7/libavcodec_2utils_8c_source.html
