# ffmpeg png生成特效的mp4 命令

只用 FFmpeg 纯命令，给你几种常用图片动画特效 MP4 命令，直接复制改文件名就能用。默认图片：filtered-%d.png（filtered-1.png、filtered-2.png…）

1）基础：淡入淡出 + 每张停留 10 秒
```bash
ffmpeg -r 0.1 -i filtered-%d.png \
  -vf "fade=in:0:10,fade=out:120:10" \
  -c:v libx264 -pix_fmt yuv420p \
  output.mp4
```
* -r 0.1：10 秒 1 张图
* fade=in:0:10：第 0 帧开始淡入，持续 10 帧
* fade=out:120:10：第 120 帧开始淡出

2）平滑转场（交叉淡入 / 滑动等）xfade 特效
```bash
ffmpeg -r 0.1 -i filtered-%d.png \
  -vf "xfade=transition=fade:duration=1:offset=9" \
  -c:v libx264 -pix_fmt yuv420p \
  transition.mp4
```
常用 transition= 替换：
* fade 淡入淡出
* slideleft 左滑
* slideright 右滑
* wipeleft 擦除
* zoom 缩放

3）Ken Burns 动态缩放（图片慢慢放大 / 移动）
```bash
ffmpeg -r 0.1 -i filtered-%d.png \
  -vf "zoompan=z='min(zoom+0.0015,1.3)':d=1:x='iw/2-(iw/zoom/2)':y='ih/2-(ih/zoom/2)'" \
  -c:v libx264 -pix_fmt yuv420p \
  zoom.mp4
```
* z= 控制放大倍数
* d=1 每帧计算一次

4）最稳通用模板（无拉伸、高清 MP4）
```bash
ffmpeg -framerate 0.1 -i filtered-%d.png \
  -vf "fade=in:0:5,fade=out:115:5" \
  -c:v libx264 -crf 18 -preset slow -pix_fmt yuv420p \
  最终视频.mp4
-crf 18：接近无损画质
-pix_fmt yuv420p：保证手机 / 播放器能播
```
