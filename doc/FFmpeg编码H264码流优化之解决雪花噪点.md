# FFmpeg编码H264码流优化之解决雪花噪点

前言：做一些相关流媒体的开发，从而接触了FFmpeg，先向雷神致敬吧！回过话题来，主要在处理程序中遇到一些FFmpeg在编码的时候一些参数的设置优化问题
文章目录
1. FFmpeg相关编码设置
1.1 FFmpeg编码流程调用函数简介
1.2 其中m_c的一些参数涉及到编码的速度和质量
1.2.1 preset 与编码速度和质量相关
1.2.2 tune 与编码速度和质量相关
1.3 I、P、B帧相关简介
1.3.1 I、P、B帧简介
1.3.2 为什么安防行业不需要B帧
1.4 博主的编码设置
2. H264编码出现花屏问题排查与解决
1. FFmpeg相关编码设置
1.1 FFmpeg编码流程调用函数简介
```
	avcodec_register_all();
	av_register_all();
    m_video_codec = avcodec_find_encoder((AVCodecID)codec_id);
    AVCodecContext *m_c = avcodec_alloc_context3(m_video_codec);
    intret1= avcodec_open2(m_c, m_video_codec, NULL);
    AVFrame *m_frame = av_frame_alloc();
    int ret2 = avcodec_encode_video2(m_c, &m_pkt, m_frame, &got_packet);
```

1.2 其中m_c的一些参数涉及到编码的速度和质量
1.2.1 preset 与编码速度和质量相关
```
av_opt_set(m_c->priv_data, “preset”, “slow”, 0); Current presets in descending order of speed are: ultrafast,superfast, veryfast, faster, fast, medium, slow, slower, veryslow,placebo.
```

priv_data 属于每个编码器特有的设置域，用av_opt_set 设置

1.2.2 tune 与编码速度和质量相关
通过--tune的参数值指定片子的类型，是和视觉优化的参数，或有特别的情况。--tune的值有
film：电影、真人类型；
animation：动画；
grain：需要保留大量的grain时用；
stillimage：静态图像编码时使用；
psnr：为提高psnr做了优化的参数；
ssim：为提高ssim做了优化的参数；
fastdecode：可以快速解码的参数；
zerolatency：零延迟，用在需要非常低的延迟的情况下，比如电视电话会议的编码。

1.3 I、P、B帧相关简介
1.3.1 I、P、B帧简介
视频解码之后，每帧都代表一幅静态的图像。而在实际压缩时，为了节省存储空间，往往会采取各种压缩算法减少数据的容量，其中I、P、B帧就是最常见的。简单地说，I帧是关键帧就是一副RGB图像，属于帧内压缩，解码时只需要利用到I帧其本身的信息即可；P帧为前向预测编码帧，即P帧解码时需要参考前面相关帧的信息才能解码；B帧为双向预测编码帧，解码时既需要参考前面已有的帧又需要参考后面待解码的帧；他们都是基于I帧来压缩数据。一般平均来说，I的压缩率是7（跟JPG图片差不多），P是20，B可以达到50，可见使用B帧能节省大量空间，节省出来的空间可以用来保存多一些I帧，这样在相同码率下，可以提供更好的画质。

I帧 帧内编码帧 又称intra picture，I 帧通常是每个 GOP（MPEG 所使用的一种视频压缩技术）的第一个帧，经过适度地压缩，做为随机访问的参考点，可以当成图象。I帧可以看成是一个图像经过压缩后的产物。I帧画面完整保留，解码时只需要本帧数据就可以完成（因为包含完整画面）。

P帧 前向预测编码帧 又称predictive-frame，通过充分将图像序列中前面已编码帧的时间冗余信息来压缩传输数据量的编码图像，也叫预测帧；表示的是这一帧跟之前的一个关键帧（或P帧）的差别，解码时需要用之前缓存的画面叠加上本帧定义的差别，生成最终画面。（也就是差别帧，P帧没有完整画面数据，只有与前一帧的画面差别的数据）

B帧 是双向差别帧，也就是B帧记录的是本帧与前后帧的差别，换言之，要解码B帧，不仅要取得之前的缓存画面，还要解码之后的画面，通过前后画面的与本帧数据的叠加取得最终的画面。B帧压缩率高，但是解码时CPU会比较累。

因此，I帧和P帧的解码算法比较简单，资源占用也比较少，I帧只要自己完成就行了，至于P帧，也只需要解码器把前一个画面缓存一下，遇到P帧时就使用之前缓存的画面就行。如果视频流只有I和P，解码器可以不管后面的数据，边读边解码，线性前进。如果视频流还有B帧，则需要缓存前面和当前的视频帧，待后面视频帧获得后，再解码。

1.3.2 为什么安防行业不需要B帧
视频监控系统中预览的视频画面是实时的，对画面的流畅性要求较高。采用I帧、P帧进行视频传输可以提高网络的适应能力，且能降低解码成本所以现阶段的视频解码都只采用I帧和P帧进行传输。I帧只需考虑本帧；P帧记录的是与前一帧的差别；B帧记录的是前一帧及后一帧的差别,能节约更多的空间,视频文件小了,但相对来说解码的时候就比较麻烦。
1.4 博主的编码设置

```
    av_opt_set(m_c->priv_data, "preset", "superfast", 0);
    av_opt_set(m_c->priv_data, "tune", "zerolatency", 0);
    m_c->max_b_frames = 0;
    m_c->has_b_frames = 0;
```

2. H264编码出现花屏问题排查与解决
在最近进行编码后使用RTSP进行推流后，发现视频出现雪花噪点，于是对编码器进行了相关排查

![image](https://user-images.githubusercontent.com/17688273/148027294-b9dc097c-acf6-4765-883e-a7d78f633752.png)


通过对m_c->bit_rate进行大小更换的排查与尝试无果

对m_c->gop_size的大小进行了排查，结果让人喜悦，当gop_size由4换为10的时候噪点消失了。

![image](https://user-images.githubusercontent.com/17688273/148027316-7a0b73b0-d3f0-4cde-a026-d122346bb7ec.png)


GOP的大小是两个最接近的I帧之间的距离，具体GOP的大小为什么影响了视频压缩的质量需要进一步学习，如果有知道的同学，欢迎赐教，谢谢！

# 参考链接
- [FFmpeg编码H264码流优化之解决雪花噪点](https://blog.csdn.net/zhouming5/article/details/115308042)
