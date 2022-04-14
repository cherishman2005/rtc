# ffmpeg推流降低延迟的优化

针对编码器上下文的参数配置
```
AVCodecContext *vc = NULL;
   // AV_CODEC_FLAG_GLOBAL_HEADER  -- 将全局头文件放在引渡文件中，而不是每个关键帧中。
   //AV_CODEC_FLAG_LOW_DELAY  --较低延迟
    vc->flags |= AV_CODEC_FLAG_GLOBAL_HEADER | AV_CODEC_FLAG_LOW_DELAY;
    
    //实时推流，零延迟
    av_opt_set(vc->priv_data, "tune", "zerolatency", 0);
    
    //一组图片的数量
    vc->gop_size = 2;
    
    //去掉B帧只留下 I帧和P帧
    vc->max_b_frames = 0;
```

# 参考链接

https://www.cnblogs.com/qiniu/p/6089097.html
https://www.cnblogs.com/jiayayao/p/7249962.html
