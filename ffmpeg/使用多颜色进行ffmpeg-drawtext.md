# 使用多颜色进行ffmpeg drawtext

要在FFmpeg中使用多颜色进行drawtext，你可以使用以下代码示例：
```
ffmpeg -i input.mp4 -vf "drawtext=text='Hello World':fontfile=/path/to/font.ttf:x=10:y=10:fontsize=30:fontcolor=red, \
drawtext=text='This is a test':fontfile=/path/to/font.ttf:x=10:y=50:fontsize=30:fontcolor=blue" output.mp4
```
上述示例命令中，我们使用了两个drawtext滤镜来添加不同颜色的文本。第一个滤镜添加了红色文本，第二个滤镜添加了蓝色文本。你可以根据需要添加更多的drawtext滤镜来使用更多的颜色。

请确保将/path/to/font.ttf替换为你自己的字体文件路径，并将input.mp4和output.mp4替换为你想要使用的输入和输出文件名。

这个示例会在视频左上角添加两行文本，第一行为红色的"Hello World"，第二行为蓝色的"This is a test"。你可以根据需要调整文本的位置、大小和颜色。
