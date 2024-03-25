# Golang调用FFmpeg转换视频流的实现

本文主要介绍了Golang调用FFmpeg转换视频流，文中通过示例代码介绍的非常详细，对大家的学习或者工作具有一定的参考学习价值。

## 问题背景

问题背景是在，由于视频采集端使用的是H264编码采集的裸流，而网络流媒体大多是以FLV为主的直播方式进行的，为了实现实时直播，当前是打算直接使用FFmpeg将H264裸流实时转成FLV视频流。

为什么是使用FLV视频流呢，因为相对简单，加上FLV Header后将每个NALU打包成Tag并进去大致就行了。但是这块怕有疏忽，最终还是想使用成熟的工具FFmpeg。


## 方法实现

### 1. 使用FFmpeg-go封装好的方法

FFmpeg-go上面有具体的Demo，但是只有流转文件，也只有文件转流的方法，对于流转流还是需要自己动手处理一下。
```
import ffmpeg "github.com/u2takey/ffmpeg-go"
  
err := ffmpeg.Input("pipe:", ffmpeg.KwArgs{
        "format":     "rawvideo",
        "video_size": fmt.Sprintf("%dx%d", 480, 1064)}).WithInput(filein).
        Output("pipe:",
            ffmpeg.KwArgs{"c:v": "libx264", "f": "flv", "crf": "24"}).
        WithOutput(buf, errorbuf).
        Run()
if err != nil {
        panic(err)
}
bufs := make([]byte, 1024)
out, _ := os.OpenFile("res2.flv", os.O_CREATE|os.O_RDWR|os.O_APPEND, 0644)
for {
    n, err := buf.Read(bufs)
    if n == 0 || err == io.EOF {
        out.Close()
        break
    } else {
        //此处处理输出流，这边简单地写到文件里
        out.Write(bufs)
    }
}
```

其中 filein 是输入的H264视频裸流   buf是输出的视频流
虽说代码是这么写，但是实际上是run不起来的，不清楚具体是什么原因，因此后续还是打算直接调用ffmpeg可执行程序。


### 2. 直接调用FFmpeg

首先需要在电脑上下载好ffmpeg并且添加到环境变量，保证执行Powshell或者cmd指令的ffmpeg时能够正常运行，接下来还是直接上代码。
```
cmd := exec.Command("ffmpeg", "-re", "-r", "30", "-i", "pipe:0", "-vcodec", "libx264", "-f", "flv", "pipe:1", "-y", "another.flv")
  
//获取输入流
stdin, err := cmd.StdinPipe()
if err != nil {
    fmt.Println("Error getting stdout pipe:", err)
    return
}
  
//要写东西进去的时候只需要
stdin.Write(bytes) 
  
//获取输出流
stdout, err := cmd.StdoutPipe()
if err != nil {
    fmt.Println("Error getting stdout pipe:", err)
    return
}
  
// Start the command
err = cmd.Start()
if err != nil {
    fmt.Println("Error starting command:", err)
    return
}
```
这边需要注意到的点是，如果不加上"-re"的话，需要等stdin.close()之后，整个ffmpeg才会运行，才能见到有输出流。

其次，这边加了 “-y” "another.flv"，是用来进行对比的，这里ffmpeg将会把输出同时放在两个地方，一个写入到文件another.flv里（-y 是覆盖原有文件），另一个通过stdout传出来，我将stdout的数据流保存成out.flv后发现同another.flv对比发现并不相同。或许视频流和文件不应该这么处理。

# 总结

建议直接调用FFmpeg的可执行文件进行操作最好。

但是这样实时的流转流的方式实际上还是有挺大延迟的，如果要实时视频流P2P模式建议要么在采集端处理好要么在播放端处理好，中间层就不要过多操作影响实时性，如果是推流拉流的模式还是建议在中间层处理好。

存在问题
这边还是发现一些问题的，尤其是直接使用ffmpeg的时候

1. 直接读取文件和读取文件通过stdin传入byte获取到的结果是不一致的。

2. 直接output成文件和通过stdout获取byte再保存成文件得到的结果也是不一致的。
