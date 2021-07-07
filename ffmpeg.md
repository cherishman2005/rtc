
# 音视频专业基础

* pts

Presentation Time Stamp。PTS主要用于度量解码后的视频帧什么时候被显示出来。

* dts

Decode Time Stamp。DTS主要是标识读入内存中的bit流在什么时候开始送入解码器中进行解码。

在没有B帧存在的情况下DTS的顺序和PTS的顺序应该是一样的。

* I/P/B帧

I frame:自身可以通过视频解压算法解压成一张单独的完整的图片。

P frame：需要参考其前面的一个I frame 或者P frame 来生成一张完整的图片。

B frame:则要参考其前一个I或者P帧及其后面的一个P帧来生成一张完整的图片。


* GOP

两个I frame之间形成一个GOP。


# 音视频解码流程

FFmpeg中用AVPacket结构体来描述解码前或编码后的压缩包，用AVFrame结构体来描述解码后或编码前的信号帧。 对于视频来说，AVFrame就是视频的一帧图像。这帧图像什么时候显示给用户，就取决于它的PTS。DTS是AVPacket里的一个成员，表示这个压缩包应该什么时候被解码。

![解封装、解码](/img/ffmpeg/video_and_audio.png)

![demux](/img/ffmpeg/demux.png)

## AVPacket

解码前或编码后的压缩包。

## AVFrame

解码后或编码前的信号帧。

# FAQ

1. 项目中一直在使用，唯一的问题就是转码时非常耗CPU，它一转码别的服务就无法运转了。如果有配置项能限制使用CPU占用就好了。

   有threads参数


# 参考链接

- [http://ffmpeg.org/](http://ffmpeg.org/)
- [FFmpeg 视频处理入门教程](http://www.ruanyifeng.com/blog/2020/01/ffmpeg.html)
- [[FFMPEG]PTS和DTS](https://www.jianshu.com/p/cc58153ac98c)