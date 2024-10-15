# FFmpeg 转码语句的preset、tune参数说明

```
ffmpeg -i input.mp4 -vcodec libx264 -threads 5 -preset medium output.mov
```

鉴于x264的参数众多，各种参数的配合复杂，为了使用者方便，x264建议如无特别需要可使用preset和tune设置。这套开发者推荐的参数较为合理，可在此基础上在调整一些具体参数以符合自己需要，手动设定的参数会覆盖preset和tune里的参数。

--preset的参数主要调节编码速度和质量的平衡，有ultrafast（转码速度最快，视频往往也最模糊）、superfast、veryfast、faster、fast、medium、slow、slower、veryslow、placebo这10个选项，从快到慢。

--tune的参数主要配合视频类型和视觉优化的参数，或特别的情况。如果视频的内容符合其中一个可用的调整值又或者有其中需要，则可以使用此选项，否则建议不使用（如tune grain是为高比特率的编码而设计的）。

tune的值有： film： 电影、真人类型；

animation： 动画；

grain： 需要保留大量的grain时用；

stillimage： 静态图像编码时使用；

psnr： 为提高psnr做了优化的参数；

ssim： 为提高ssim做了优化的参数；

fastdecode： 可以快速解码的参数；

zerolatency：零延迟，用在需要非常低的延迟的情况下，比如电视电话会议的编码。
