# ffmpeg常见使用

## 调整播放速度

加速四倍：
```
ffmpeg -i TheOrigin.mp4 -vf  "setpts=0.25*PTS" UpTheOrigin.mp4
```

四倍慢速：
```
ffmpeg -i TheOrigin.mp4 -vf  "setpts=4*PTS" DownTheOrigin.mp4
```

# FAQ

## 音频播放过快，视频慢，如何选择丢帧

* 优先选择丢B帧

* 如果挤压太多可以考虑丢1个GOP范围内地所有帧。

最基本的原则就是不能影响到一个GOP内的解码。

# 参考链接
