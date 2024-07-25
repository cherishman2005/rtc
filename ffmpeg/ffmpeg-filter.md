# ffmpeg-filter

## scale=78:24,transpose=cclock

```
"scale=78:24,transpose=cclock"
```

在 FFmpeg 中，scale 和 transpose 是两个常用的视频滤镜。scale 滤镜用于调整视频帧的大小，而 transpose 滤镜用于旋转或翻转视频帧。

你给出的滤镜链 "scale=78:24,transpose=cclock" 包含两个滤镜，其意义如下：

scale=78:24: 这个滤镜将视频帧的尺寸调整为宽度 78 像素和高度 24 像素。这是一个非常小的尺寸，通常用于创建缩略图或特殊用途的视频。

transpose=cclock: 这个滤镜将视频帧逆时针旋转 90 度（counterclockwise）。cclock 是 transpose 滤镜的一个选项，表示逆时针旋转。

将这两个滤镜组合在一起，意味着首先视频帧会被缩小到 78x24 像素，然后这个缩小后的帧会被逆时针旋转 90 度。

在 FFmpeg 命令行中使用这个滤镜链的示例：

```bash
ffmpeg -i input.mp4 -vf "scale=78:24,transpose=cclock" output.mp4
```

这里：

* -i input.mp4 指定输入视频文件。
* -vf 指定视频滤镜链。
* "scale=78:24,transpose=cclock" 是滤镜链，包含 scale 和 transpose 两个滤镜。
* output.mp4 是输出视频文件。

注意：由于先进行了尺寸缩小，然后再进行旋转，所以旋转后的视频帧的宽度和高度会与原始尺寸不同（在这种情况下，旋转后的尺寸将是 24x78 像素，但宽度和高度标签可能仍然显示为 78x24，因为它们是基于原始尺寸的）。如果你需要保持特定的输出尺寸，你可能需要在旋转后再次应用 scale 滤镜来调整尺寸。
