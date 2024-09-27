# ffmpeg-shortest功能

在FFmpeg中，int shortest是用于设置最短输入流的参数。当使用FFmpeg进行音视频处理时，可能会涉及到多个输入流，这些输入流在完全处理完成之前可能会有不同长度的情况。shortest参数的作用是告诉FFmpeg在处理多个输入流时，以最短输入流的长度作为输出流的长度，以确保多个输入流能够完全匹配。

具体来说，shortest参数用于在执行FFmpeg命令时指定是否应该以最短输入流的长度为准。通过设置shortest参数，您可以控制FFmpeg在处理视频或音频时应该遵循哪个输入流的长度，以确保输出流的长度与最短输入流的长度相匹配。

以下是一个示例命令，展示了如何在FFmpeg命令中使用shortest参数：

```Bash
ffmpeg -i input1.mp4 -i input2.mp4 -shortest output.mp4
```

在上述示例中，通过指定-shortest参数，FFmpeg将根据最短输入流的长度来确定输出流的长度。这样可以避免输出流过长导致处理不一致的问题。

总之，shortest参数是一个有用的选项，可以在处理多个输入流时确保输出流的长度与最短输入流的长度保持一致。
