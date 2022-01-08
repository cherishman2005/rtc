# node-fluent-ffmpeg 文档

node-fluent-ffmpeg 该库将 ffmpeg 的复杂命令行用法抽象为一个流畅，易于使用的 node.js 模块。为了能够使用此模块，请确保在系统上安装了ffmpeg（包括所有必需的编码库，如 libmp3lame 或 libx264）。

安装
npm
```
$ npm install fluent-ffmpeg
```
yarn
```
$ yarn add fluent-ffmpeg
```
或者作为一个 submodule

$ git submodule add git://github.com/schaermu/node-fluent-ffmpeg.git vendor/fluent-ffmpeg
用法
你将在 examples 文件夹中找到一些有用的例子。

先决条件
ffmpeg and ffprobe
fluent-ffmpeg 需要 ffmpeg> = 0.9 才能工作。它可能与以前的版本一起使用，但是一些功能将不可用（并且不再对库进行较低版本的测试）。

如果设置了 FFMPEG_PATH 环境变量，fluent-ffmpeg 会将其用作 ffmpeg 可执行文件的完整路径。否则，它将尝试直接调用 ffmpeg（因此它应该在您的 PATH 中）。您还必须安装 ffprobe（大多数发行版随附 ffmpeg）。同样，fluent-ffmpeg 将使用 FFPROBE_PATH 环境变量（如果已设置），否则它将尝试在 PATH 中调用它。

当使用 avconv 和 avprobe 而不是 ffmpeg 和 ffprobe 时，大多数功能都可以使用，但是目前尚不正式支持。

Windows 用户：ffmpeg 和 ffprobe 很可能不在您的％PATH 中，因此您必须设置％FFMPEG_PATH 和％FFPROBE_PATH。

Debian / Ubuntu 用户：官方存储库在 libav-tools 软件包中具有 ffmpeg / ffprobe 可执行文件，并且它们实际上是重命名为 avconv / avprobe 可执行文件（avconv 是 ffmpeg 的分支）。它们应该基本兼容，但是如果您遇到任何问题，则可能要使用真正的 ffmpeg。您可以从源代码编译它，也可以在 https://ffmpeg.org/download.html 上找到预构建的.deb包（对于Ubuntu，ppa：mc3man / trusty-media PPA 提供了最新的版本）。

flvtool2 or flvmeta
如果要编码 FLV 视频，则必须安装 flvtool2 或 flvmeta，并且在 PATH 或 fluent-ffmpeg 中将无法生成可流式输出文件。如果您设置了 FLVTOOL2_PATH 或 FLVMETA_PATH，fluent-ffmpeg 将尝试使用它而不是在 PATH 中搜索。

Setting binary paths manually
另外，您可以使用以下 API 命令手动设置 ffmpeg，ffprobe 和 flvtool2 / flvmeta 二进制路径：

**Ffmpeg.setFfmpegPath(path)**参数路径是具有 ffmpeg 二进制文件完整路径的字符串。
**Ffmpeg.setFfprobePath(path)**参数路径是具有 ffprobe 二进制文件完整路径的字符串。
**Ffmpeg.setFlvtoolPath(path)**参数路径是具有 flvtool2 或 flvmeta 二进制文件的完整路径的字符串。
Creating an FFmpeg command
fluent-ffmpeg 模块返回可用于实例化 FFmpeg 命令的构造函数。
```
var FfmpegCommand = require('fluent-ffmpeg')
var command = new FfmpegCommand()
```
您也可以不使用 new 运算符而使用构造函数。
```
var ffmpeg = require('fluent-ffmpeg')
var command = ffmpeg()
```
您可以将输入文件名或可读流，配置对象或两者都传递给构造函数。
```
var command = ffmpeg('/path/to/file.avi');
var command = ffmpeg(fs.createReadStream('/path/to/file.avi'));
var command = ffmpeg({ option: "value", ... });
var command = ffmpeg('/path/to/file.avi', { option: "value", ... });
```
提供以下选项：

source: 输入文件名或可读流（如果将输入文件传递给构造函数则忽略）
timeout: ffmpeg 超时（以秒为单位）（默认为无超时）
preset 或 presets: 用于从中加载模块预设的目录（默认为 fluent-ffmpeg 树中的 lib / presets 目录）
niceness 或 priority: ffmpeg 优美度值，介于-20 和 20 之间；在 Windows 平台上被忽略（默认为 0）
logger: 带有 debug()，info()，warn()和 error()方法的记录器对象（默认为不记录）
stdoutLines: ffmpeg stdout / stderr 在内存中保留的最大行数（默认为 100，使用 0 表示无限存储）
Specifying inputs
您可以向 Ffmpeg 命令添加任意数量的输入。输入可以是：

文件名（例如/path/to/file.avi）；
图像正则表达式（例如/path/to/frame%03d.png）；
可读流;一个命令只能使用一个输入流，但是您可以同时使用一个输入流和一个或多个文件名。
```
// Note that all fluent-ffmpeg methods are chainable
ffmpeg('/path/to/input1.avi')
  .input('/path/to/input2.avi')
  .input(fs.createReadStream('/path/to/input3.avi'))
// Passing an input to the constructor is the same as calling .input()
ffmpeg()
  .input('/path/to/input1.avi')
  .input('/path/to/input2.avi')
// Most methods have several aliases, here you may use addInput or mergeAdd instead
ffmpeg()
  .addInput('/path/to/frame%02d.png')
  .addInput('/path/to/soundtrack.mp3')
ffmpeg()
  .mergeAdd('/path/to/input1.avi')
  .mergeAdd('/path/to/input2.avi')
```
Input options
以下方法可以将与输入有关的设置传递给 ffmpeg。这些方法中的每一个都适用于最后添加的输入（包括传递给构造函数的输入，如果有的话）。您必须在调用这些设置之前添加输入，否则将引发错误。

inputFormat(format): 指定输入格式
Aliases: fromFormat(), withInputFormat() 这仅对原始输入有用，因为 ffmpeg 可以自动确定输入格式。
```
ffmpeg()
  .input('/dev/video0')
  .inputFormat('mov')
  .input('/path/to/file.avi')
  .inputFormat('avi')
```
Fluent-ffmpeg 在实际运行命令之前检查格式是否可用，并在指定的输入格式不可用时引发错误。

inputFPS(fps): 指定输入帧率
Aliases: withInputFps(), withInputFPS(), withFpsInput(), withFPSInput(), inputFps(), fpsInput(), FPSInput() 这仅对原始输入有效，因为 ffmpeg 可以自动确定输入帧率。
```
ffmpeg('/dev/video0').inputFPS(29.7)
native(): 以本地帧速率读取输入
Aliases: nativeFramerate(), withNativeFramerate()

ffmpeg('/path/to/file.avi').native()
seekInput(time): 设置输入开始时间
Alias: setStartTime() 寻求输入，仅在给定的时间偏移开始解码。 time 参数可以是数字（以秒为单位）或时间戳字符串（格式为[[hh:]mm:]ss[.xxx]）。

ffmpeg('/path/to/file.avi').seekInput(134.5)
ffmpeg('/path/to/file.avi').seekInput('2:14.500')
loop([duration]): 循环输入
ffmpeg('/path/to/file.avi').loop()
ffmpeg('/path/to/file.avi').loop(134.5)
ffmpeg('/path/to/file.avi').loop('2:14.500')
```
inputOptions(option…): 添加自定义输入选项
Aliases: inputOption(), addInputOption(), addInputOptions(), withInputOption(), withInputOptions() 此方法允许将任何与输入有关的选项传递给 ffmpeg。您可以使用单个参数来调用它以传递单个选项，还可以选择使用空格分隔的参数：
```
/* Single option */
ffmpeg('/path/to/file.avi').inputOptions('-someOption')
/* Single option with parameter */
ffmpeg('/dev/video0').inputOptions('-r 24')
```
您也可以通过将数组传递给方法来一次传递多个选项：
```
ffmpeg('/path/to/file.avi').inputOptions([
  '-option1',
  '-option2 param2',
  '-option3',
  '-option4 param4'
])
```
最后，您还可以直接将命令行标记作为单独的参数传递给方法：
```
ffmpeg('/path/to/file.avi').inputOptions(
  '-option1',
  '-option2',
  'param2',
  '-option3',
  '-option4',
  'param4'
)
```
Audio options
下列方法更改产生的输出中的音频流。

noAudio(): 完全禁用音频
Aliases: withNoAudio() 禁用输出中的音频，并删除任何以前设置的音频选项。

ffmpeg('/path/to/file.avi').noAudio()
audioCodec(codec): 设置音频编解码器
Aliases: withAudioCodec()
```
ffmpeg('/path/to/file.avi').audioCodec('libmp3lame')
```
Fluent-ffmpeg 在实际运行命令之前检查编解码器是否可用，并在指定的音频编解码器不可用时引发错误。

audioBitrate(bitrate): 设置音频比特率
Aliases: withAudioBitrate() 以 kbps 设置音频比特率。 bitrate参数可以是数字或带可选 k 后缀的字符串。此方法用于强制执行恒定的比特率。使用audioQuality()使用可变比特率进行编码
```
ffmpeg('/path/to/file.avi').audioBitrate(128)
ffmpeg('/path/to/file.avi').audioBitrate('128')
ffmpeg('/path/to/file.avi').audioBitrate('128k')
```
audioChannels(count): 设置音频通道数
Aliases: withAudioChannels()
```
ffmpeg('/path/to/file.avi').audioChannels(2)
```
audioFrequency(freq): 设置音频频率
Aliases: withAudioFrequency() freq 参数指定以 Hz 为单位的音频频率
```
ffmpeg('/path/to/file.avi').audioFrequency(22050)
```
audioQuality(quality): 设置音频质量
Aliases: withAudioQuality() 此方法固定音频编解码器（VBR 编码）的质量因数。质量等级取决于实际使用的编解码器。
```
ffmpeg('/path/to/file.avi')
  .audioCodec('libmp3lame')
  .audioQuality(0)
```
audioFilters(filter…): 添加自定义音频过滤器
Aliases: audioFilter(), withAudioFilter(), withAudioFilters() 此方法可以添加自定义音频过滤器。您可以通过传递多个参数或数组来一次添加多个过滤器。有关可用的过滤器及其语法，请参见 Ffmpeg 文档。 传到此方法的每个过滤器可以是过滤器字符串（例如，volume=0.5）或具有以下键的过滤器规范对象：

filter: 过滤器名称
options: 可选的;过滤器的选项字符串（例如 n=-50dB:d=5），未命名选项的选项数组(eg. ['-50dB', 5])或对象将选项名称映射到值(eg. { n: ‘-50dB’, d: 5 })。未指定options时，将添加不带任何设置的过滤器。
```
ffmpeg('/path/to/file.avi')
  .audioFilters('volume=0.5')
  .audioFilters('silencedetect=n=-50dB:d=5');
ffmpeg('/path/to/file.avi')
  .audioFilters('volume=0.5', 'silencedetect=n=-50dB:d=5');
ffmpeg('/path/to/file.avi')
  .audioFilters(['volume=0.5', 'silencedetect=n=-50dB:d=5']);
ffmpeg('/path/to/file.avi')
  .audioFilters([
    {
      filter: 'volume',
      options: '0.5'
    },
    {
      filter: 'silencedetect',
      options: 'n=-50dB:d=5'
    }
  ]);
ffmpeg('/path/to/file.avi')
  .audioFilters(
    {
      filter: 'volume',
      options: ['0.5']
    },
    {
      filter: 'silencedetect',
      options: { n: '-50dB', d: 5 }
    }
  ]);
```
Video options
下列方法更改产生的输出中的视频流。

noVideo(): 完全禁用视频
Aliases: withNoVideo() 此方法禁用视频输出，并删除任何以前设置的视频选项。

ffmpeg('/path/to/file.avi').noVideo()
videoCodec(codec): 设置视频编解码器
Aliases: withVideoCodec()

ffmpeg('/path/to/file.avi').videoCodec('libx264')
Fluent-ffmpeg 在实际运行命令之前检查编解码器的可用性，并在指定的视频编解码器不可用时引发错误。

videoBitrate(bitrate[, constant=false]): 设置视频比特率
Aliases: withVideoBitrate() 设置目标视频比特率（以 kbps 为单位）。 bitrate参数可以是数字或带可选k后缀的字符串。 constant参数指定是否应实施恒定比特率（默认为false）。 请记住，根据所使用的编解码器，强制采用恒定比特率通常会牺牲质量。保持恒定视频比特率而不损失太多质量的最佳方法是使用 2-pass 编码（请参阅 Fffmpeg 文档）。
```
ffmpeg('/path/to/file.avi').videoBitrate(1000)
ffmpeg('/path/to/file.avi').videoBitrate('1000')
ffmpeg('/path/to/file.avi').videoBitrate('1000k')
ffmpeg('/path/to/file.avi').videoBitrate('1000k', true)
```
videoFilters(filter…): 添加自定义视频过滤器
Aliases: videoFilter(), withVideoFilter(), withVideoFilters() 此方法可以添加自定义视频过滤器。您可以通过传递多个参数或数组来一次添加多个过滤器。有关可用的过滤器及其语法，请参见 Ffmpeg 文档。 传到此方法的每个过滤器可以是过滤器字符串（例如，volume=0.5）或具有以下键的过滤器规范对象：

filter: 过滤器名称
options: 可选的;过滤器的选项字符串 (eg. in:0:30)，未命名选项的选项数组 (eg. [‘in’, 0, 30])或对象将选项名称映射到值(eg. { t: ‘in’, s: 0, n: 30 })。未指定options时，将添加不带任何选项的过滤器。
```
ffmpeg('/path/to/file.avi')
  .videoFilters('fade=in:0:30')
  .videoFilters('pad=640:480:0:40:violet')
ffmpeg('/path/to/file.avi').videoFilters(
  'fade=in:0:30',
  'pad=640:480:0:40:violet'
)
ffmpeg('/path/to/file.avi').videoFilters([
  'fade=in:0:30',
  'pad=640:480:0:40:violet'
])
ffmpeg('/path/to/file.avi').videoFilters([
  {
    filter: 'fade',
    options: 'in:0:30'
  },
  {
    filter: 'pad',
    options: '640:480:0:40:violet'
  }
])
ffmpeg('/path/to/file.avi').videoFilters(
  {
    filter: 'fade',
    options: ['in', 0, 30]
  },
  {
    filter: 'filter2',
    options: { w: 640, h: 480, x: 0, y: 40, color: 'violet' }
  }
)
```
fps(fps): 设置输出帧率
Aliases: withOutputFps(), withOutputFPS(), withFpsOutput(), withFPSOutput(), withFps(), withFPS(), outputFPS(), outputFps(), fpsOutput(), FPSOutput(), FPS()

ffmpeg('/path/to/file.avi').fps(29.7)
frames(count): 指定帧数 Aliases: takeFrames(), withFrames() 将 ffmpeg 设置为仅编码一定数量的帧。

ffmpeg('/path/to/file.avi').frames(240)
视频帧尺寸 options
通过以下方法可以调整输出视频帧大小的尺寸。它们一起工作以生成适当的视频过滤器

size(size): 设置输出帧大小
Aliases: videoSize(), withSize() 此方法设置输出帧的大小。 size参数可能具有以下格式之一：

640x480: 设置固定的输出帧大小。除非调用autopad()，否则可能会导致视频被拉伸或压缩以适合请求的大小。
640x?: 设置固定宽度并自动计算高度。如果aspect()也被调用，则用于计算视频高度；否则进行计算，以便保留输入的宽高比。
?x480: 设置一个固定的高度并自动计算宽度。如果aspect()也被调用，则用于计算视频宽度；否则进行计算，以便保留输入的宽高比。
50%: 将宽度和高度都缩放到给定的百分比。纵横比始终保持不变
请注意，为了与某些编解码器兼容，计算尺寸总是四舍五入为 2 的倍数。
```
ffmpeg('/path/to/file.avi').size('640x480')
ffmpeg('/path/to/file.avi').size('640x?')
ffmpeg('/path/to/file.avi')
  .size('640x?')
  .aspect('4:3')
ffmpeg('/path/to/file.avi').size('50%')
```
aspect(aspect): 设置输出帧的宽高比
Aliases: withAspect(), withAspectRatio(), setAspect(), setAspectRatio(), aspectRatio() 此方法强制执行特定的输出宽高比。 aspect参数可以是数字或X：Y字符串。 请注意，当以固定的宽度和高度或百分比调用size()时，以及根本没有调用size()时，将忽略对aspect()的调用。
```
ffmpeg('/path/to/file.avi')
  .size('640x?')
  .aspect('4:3')
ffmpeg('/path/to/file.avi')
  .size('640x?')
  .aspect(1.33333)
```
autopad([color=‘black’]): 启用自动填充输出视频
Aliases: applyAutopadding(), applyAutoPadding(), applyAutopad(), applyAutoPad(), withAutopadding(), withAutoPadding(), withAutopad(), withAutoPad(), autoPad() 此方法可以将自动填充应用于输出视频。 color参数指定用于填充的颜色，并且必须是 ffmpeg 支持的颜色代码或名称（默认为’black'）。 此方法的行为取决于对其他视频尺寸方法的调用：

当size()已被调用或未调用时，将被忽略；
当用 WxH 调用 size() 时，它会添加填充以保持输入的长宽比；
当用任一Wx?或?xH调用size()时,仅当调用aspect()时才添加填充（否则，将根据输入的宽高比计算输出尺寸，而无需填充）。
```
// No size specified, autopad() is ignored
// 当`size()`已被调用或未调用时，将被忽略；
ffmpeg('/path/to/file.avi').autopad()
// 添加填充以保持原始宽高比。
// - with a 640x400 input, 40 pixels of padding are added on both sides
// - with a 600x480 input, 20 pixels of padding are added on top and bottom
// - with a 320x200 input, video is scaled up to 640x400 and 40px of padding
//   is added on both sides
// - with a 320x240 input, video is scaled up to 640x480 and and no padding
//   is needed
ffmpeg('/path/to/file.avi')
  .size('640x480')
  .autopad()
ffmpeg('/path/to/file.avi')
  .size('640x480')
  .autopad('white')
ffmpeg('/path/to/file.avi')
  .size('640x480')
  .autopad('#35A5FF')
// Size computed from input, autopad() is ignored
ffmpeg('/path/to/file.avi')
  .size('50%')
  .autopad()
ffmpeg('/path/to/file.avi')
  .size('640x?')
  .autopad()
ffmpeg('/path/to/file.avi')
  .size('?x480')
  .autopad()
// 调用 .size('640x?').aspect('4:3') 类似于调用 .size('640x480')
// - 输入为640x400时，两侧均添加40像素的填充
// - with a 600x480 input, 20 pixels of padding are added on top and bottom
// - with a 320x200 input, video is scaled up to 640x400 and 40px of padding
//   is added on both sides
// - with a 320x240 input, video is scaled up to 640x480 and and no padding
//   is needed
ffmpeg('/path/to/file.avi')
  .size('640x?')
  .aspect('4:3')
  .autopad()
ffmpeg('/path/to/file.avi')
  .size('640x?')
  .aspect('4:3')
  .autopad('white')
ffmpeg('/path/to/file.avi')
  .size('640x?')
  .aspect('4:3')
  .autopad('#35A5FF')
// Calling .size('?x480').aspect('4:3') is similar to calling .size('640x480')
ffmpeg('/path/to/file.avi')
  .size('?x480')
  .aspect('4:3')
  .autopad()
ffmpeg('/path/to/file.avi')
  .size('?x480')
  .aspect('4:3')
  .autopad('white')
ffmpeg('/path/to/file.avi')
  .size('?x480')
  .aspect('4:3')
  .autopad('#35A5FF')
```
为了与以前的 fluent-ffmpeg 版本兼容，此方法还接受一个附加的布尔第一参数，该参数指定是否应用自动填充。

ffmpeg('/path/to/file.avi')
  .size('640x480')
  .autopad(true)
ffmpeg('/path/to/file.avi')
  .size('640x480')
  .autopad(true, 'pink')
keepDAR(): 保持显示纵横比
Aliases: keepPixelAspect(), keepDisplayAspect(), keepDisplayAspectRatio() 当将具有非正方形像素的输入转换为不支持非正方形像素的输出格式（例如，大多数图像格式）时，此方法很有用。它会重新缩放输入，以使显示宽高比相同。

ffmpeg('/path/to/file.avi').keepDAR()
Specifying multiple outputs
output(target[, options]): 将输出添加到命令
Aliases: addOutput() 将输出添加到命令。target 参数可以是输出文件名或可写流（但单个命令最多可以使用一个输出流）。

当target是流时，可以传递其他options对象。如果存在，它将通过 ffmpeg 输出流pipe()方法传递。

添加输出将切换命令的“当前输出”，以便将应用于该输出的任何 fluent-ffmpeg 方法确实应用于最后添加的输出。出于向后兼容的原因，您最好在添加第一个输出之前调用这些方法（在这种情况下，它们将在添加第一个输出时应用于第一个输出）。适用于输出的方法是所有与输入无关的方法，但complexFilter()是全局的。

还要注意，在调用output()时，不应使用save()或stream()（以前称为 saveToFile（）和 writeToStream（））方法，因为它们已经添加了输出。使用run()方法开始处理。
```
var stream = fs.createWriteStream('outputfile.divx')
ffmpeg('/path/to/file.avi')
  .output('outputfile.mp4')
  .output(stream)
ffmpeg('/path/to/file.avi')
  // You may pass a pipe() options object when using a stream
  .output(stream, { end: true })
// Output-related methods apply to the last output added
ffmpeg('/path/to/file.avi')
  .output('outputfile.mp4')
  .audioCodec('libfaac')
  .videoCodec('libx264')
  .size('320x200')
  .output(stream)
  .preset('divx')
  .size('640x480')
// Use the run() method to run commands with multiple outputs
ffmpeg('/path/to/file.avi')
  .output('outputfile.mp4')
  .output(stream)
  .on('end', function() {
    console.log('Finished processing')
  })
  .run()
```
Output options
duration(time): 设置输出持续时间
Aliases: withDuration(), setDuration() 强制 ffmpeg 在特定的输出持续时间后停止转码。 time 参数可以是数字（以秒为单位）或时间戳字符串（格式为[[hh:]mm:]ss[.xxx]）。
```
ffmpeg('/path/to/file.avi').duration(134.5)
ffmpeg('/path/to/file.avi').duration('2:14.500')
```
seek(time): 搜寻输出
Aliases: seekOutput() 在将流编码到输出之前先查找流。这与调用seekInput()不同，因为偏移量仅适用于一个输出。这也较慢，因为跳过的帧仍将被解码（但丢弃）。 time 参数可以是数字（以秒为单位）或时间戳字符串（格式为[[hh:]mm:]ss[.xxx]）。
```
ffmpeg('/path/to/file.avi')
  .seekInput('1:00')
  .output('from-1m30s.avi')
  .seek(30)
  .output('from-1m40s.avi')
  .seek('0:40')
```
format(format): 设置输出格式
Aliases: withOutputFormat(), toFormat(), outputFormat()
```
ffmpeg('/path/to/file.avi').format('flv')
```
flvmeta(): 转码后更新 FLV 元数据
Aliases: updateFlvMetadata() 调用此方法可使 fluent-ffmpeg 在输出文件上运行flvmeta或flvtool2，以添加 FLV 元数据并使文件可流式传输。当输出到流时，它不起作用，仅在输出为 FLV 格式时才有用。
```
ffmpeg('/path/to/file.avi')
  .flvmeta()
  .format('flv')
```
outputOptions(option…): 添加自定义输出选项
Aliases: outputOption(), addOutputOption(), addOutputOptions(), withOutputOption(), withOutputOptions(), addOption(), addOptions() 此方法允许将任何与输出相关的选项传递给 ffmpeg。您可以使用单个参数来调用它以传递单个选项，还可以选择使用空格分隔的参数：
```
/* Single option */
ffmpeg('/path/to/file.avi').outputOptions('-someOption')
/* Single option with parameter */
ffmpeg('/dev/video0').outputOptions('-r 24')
```
您也可以通过将数组传递给方法来一次传递多个选项：
```
ffmpeg('/path/to/file.avi').outputOptions([
  '-option1',
  '-option2 param2',
  '-option3',
  '-option4 param4'
])
```
最后，您还可以直接将命令行标记作为单独的参数传递给方法：
```
ffmpeg('/path/to/file.avi').outputOptions(
  '-option1',
  '-option2',
  'param2',
  '-option3',
  '-option4',
  'param4'
)
```
Miscellaneous options
preset(preset): 使用 fluent-ffmpeg 预设
Aliases: usingPreset() fluent-ffmpeg 支持两种预设。第一个是预设模块；要使用这些名称，请将预设名称作为预设参数传递。预设模块从presets构造函数选项指定的目录中加载（默认为lib/presets fluent-ffmpeg 子目录）。
```
// Uses <path-to-fluent-ffmpeg>/lib/presets/divx.js
ffmpeg('/path/to/file.avi').preset('divx')
// Uses /my/presets/foo.js
ffmpeg('/path/to/file.avi', { presets: '/my/presets' }).preset('foo')
```
预设模块必须导出以 FfmpegCommand 作为参数的load()函数。 fluent-ffmpeg 预先安装了以下预设模块：

divx
flashvideo
podcast 以下是包含的divx预设中的代码作为示例：
```
exports.load = function(ffmpeg) {
  ffmpeg
    .format('avi')
    .videoBitrate('1024k')
    .videoCodec('mpeg4')
    .size('720x?')
    .audioBitrate('128k')
    .audioChannels(2)
    .audioCodec('libmp3lame')
    .outputOptions(['-vtag DIVX'])
}
```
第二种预设是预设函数。要使用这些函数，请传递一个以 FfmpegCommand 作为参数的函数。
```
function myPreset(command) {
  command.format('avi').size('720x?')
}
ffmpeg('/path/to/file.avi').preset(myPreset)
```
complexFilter(filters[, map]): 设置复杂的 filtergraph
Aliases: filterGraph() complexFilter()方法可为命令设置复杂的过滤器图。它期望一个过滤器规范（或过滤器规范数组）和一个可选的输出映射参数作为参数。 过滤器规范可以是简单的 ffmpeg 过滤器字符串（例如 split=3[a][b][c]）或具有以下键的对象：

filter: 过滤器名称
options: 可选的;过滤器的选项字符串 (eg. in:0:30)，未命名选项的选项数组(eg. [‘in’, 0, 30]) 或对象将选项名称映射到值(eg. { t: ‘in’, s: 0, n: 30 })。未指定选项时，将添加不带任何选项的过滤器。
inputs: 可选的;过滤器的输入流说明符。该值可以是单个流说明符字符串，也可以是流说明符数组。每个说明符都可以选择包含在方括号中。如果未指定输入流，则 ffmpeg 将使用正确类型的第一个未使用的流。
outputs: 可选的;过滤器的输出流说明符。该值可以是单个流说明符字符串，也可以是流说明符数组。每个说明符都可以选择包含在方括号中。
输出映射参数指定要在过滤器图的输出中包括哪些流。它可以是单个流说明符字符串，也可以是流说明符数组。每个说明符都可以选择包含在方括号中。如果不存在此参数，则 ffmpeg 将默认将所有未使用的输出保存到输出文件。

注意，在给定命令上只能设置一个复杂的过滤图。再次调用 complexFilter（）将覆盖以前设置的任何过滤器图形，但是您可以在单个调用中根据需要设置任意多个过滤器。
```
ffmpeg('/path/to/file.avi').complexFilter(
  [
    // Rescale input stream into stream 'rescaled'
    'scale=640:480[rescaled]',
    // Duplicate rescaled stream 3 times into streams a, b, and c
    {
      filter: 'split',
      options: '3',
      inputs: 'rescaled',
      outputs: ['a', 'b', 'c']
    },
    // Create stream 'red' by removing green and blue channels from stream 'a'
    {
      filter: 'lutrgb',
      options: { g: 0, b: 0 },
      inputs: 'a',
      outputs: 'red'
    },
    // Create stream 'green' by removing red and blue channels from stream 'b'
    {
      filter: 'lutrgb',
      options: { r: 0, b: 0 },
      inputs: 'b',
      outputs: 'green'
    },
    // Create stream 'blue' by removing red and green channels from stream 'c'
    {
      filter: 'lutrgb',
      options: { r: 0, g: 0 },
      inputs: 'c',
      outputs: 'blue'
    },
    // Pad stream 'red' to 3x width, keeping the video on the left,
    // and name output 'padded'
    {
      filter: 'pad',
      options: { w: 'iw*3', h: 'ih' },
      inputs: 'red',
      outputs: 'padded'
    },
    // Overlay 'green' onto 'padded', moving it to the center,
    // and name output 'redgreen'
    {
      filter: 'overlay',
      options: { x: 'w', y: 0 },
      inputs: ['padded', 'green'],
      outputs: 'redgreen'
    },
    // Overlay 'blue' onto 'redgreen', moving it to the right
    {
      filter: 'overlay',
      options: { x: '2*w', y: 0 },
      inputs: ['redgreen', 'blue'],
      outputs: 'output'
    }
  ],
  'output'
)
```
Setting event handlers
在实际运行命令之前，您可能希望在其上设置事件侦听器，以在完成操作时得到通知。以下事件可用：

‘start’: ffmpeg 进程开始
生成 ffmpeg 之后立即发出 start 事件。它以完整的命令行作为参数发出。
```
ffmpeg('/path/to/file.avi').on('start', function(commandLine) {
  console.log('Spawned Ffmpeg with command: ' + commandLine)
})
```
‘codecData’: 输入编解码器数据可用
当 ffmpeg 输出有关其输入流的编解码器信息时，将发出 codecData 事件。它与带有以下键的 object 参数一起发出：

format: 输入格式
duration: 输入时间
audio: 音频编解码器
audio_details: 音频编码详细信息
video: 视频编解码器
video_details: 视频编码详细信息
```
ffmpeg('/path/to/file.avi').on('codecData', function(data) {
  console.log(
    'Input is ' + data.audio + ' audio ' + 'with ' + data.video + ' video'
  )
})
```
‘progress’: 转码进度信息
每当 ffmpeg 报告进度信息时，都会发出进度事件。它与带有以下键的 object 参数一起发出：

frames: 已处理的总帧数
currentFps: FFmpeg 当前正在处理的帧速率
currentKbps: FFmpeg 当前正在处理的吞吐量
targetSize: 目标文件的当前大小（以千字节为单位）
timemark: 当前帧的时间戳（以秒为单位）
percent: 进度百分比的估计
请注意，percent可能（非常）不准确，因为 fluent-ffmpeg 从 ffmpeg 获得的唯一进度信息是写入的帧总数（以及相应的持续时间）。要估算百分比，fluent-ffmpeg 必须猜测总输出持续时间是多少，并使用添加到命令中的第一个输入来执行此操作。特别是：

使用输入流时百分比不可用
当使用多个具有不同持续时间的输入时，百分比可能是错误的，并且第一个不是最长的
```
ffmpeg('/path/to/file.avi').on('progress', function(progress) {
  console.log('Processing: ' + progress.percent + '% done')
})
```
‘stderr’: FFmpeg 输出
每次 FFmpeg 向stderr输出一条线时，都会发出stderr事件。它与包含 stderr 的行的字符串一起发出（减去结尾的换行符）。

ffmpeg('/path/to/file.avi').on('stderr', function(stderrLine) {
  console.log('Stderr output: ' + stderrLine)
})
‘error’: 转码错误
运行 ffmpeg 或准备执行 ffmpeg 时发生错误时，会发出error事件。它以错误对象作为参数发出。如果在 ffmpeg 执行过程中发生了错误，侦听器还将收到另外两个包含 ffmpegs stdout 和 stderr 的参数。 如果将流用于输入或输出，则从这些流发出的任何错误都将传递到此事件，并分别作为输入和输出流的inputStreamError和outputStreamError附加到该error。 警告：您应该始终为错误事件设置处理程序，因为在发出没有任何侦听器的错误事件时，节点的默认行为是将错误输出到控制台并终止程序。
```
ffmpeg('/path/to/file.avi').on('error', function(err, stdout, stderr) {
  console.log('Cannot process video: ' + err.message)
})
```
‘end’: 处理完成
处理完成后，发出结束事件。侦听器接收 ffmpeg 标准输出和标准错误作为参数，但生成缩略图时除外（请参见下文），在这种情况下，侦听器将接收生成的文件名数组。
```
ffmpeg('/path/to/file.avi').on('end', function(stdout, stderr) {
  console.log('Transcoding succeeded !')
})
```
命令输出到流时，stdout为空。 stdout和stderr都受stdoutLines选项限制（默认为 100 行）。

Starting FFmpeg processing
save(filename): 将输出保存到文件
Aliases: saveToFile() 开始 ffmpeg 处理并将输出保存到文件中。
```
ffmpeg('/path/to/file.avi')
  .videoCodec('libx264')
  .audioCodec('libmp3lame')
  .size('320x240')
  .on('error', function(err) {
    console.log('An error occurred: ' + err.message)
  })
  .on('end', function() {
    console.log('Processing finished !')
  })
  .save('/path/to/output.mp4')
```
注意：save()方法实际上是用于调用output()和run()的语法糖。

pipe([stream], [options]): 将输出通过管道传输到可写流
Aliases: stream(), writeToStream() 开始处理并将 ffmpeg 输出传送到可写流。如果存在 options 参数，则将其传递给 ffmpeg 输出流的 pipe（）方法（请参阅 nodejs 文档）。
```
var outStream = fs.createWriteStream('/path/to/output.mp4')
ffmpeg('/path/to/file.avi')
  .videoCodec('libx264')
  .audioCodec('libmp3lame')
  .size('320x240')
  .on('error', function(err) {
    console.log('An error occurred: ' + err.message)
  })
  .on('end', function() {
    console.log('Processing finished !')
  })
  .pipe(outStream, { end: true })
```
当不存在任何流参数时，pipe()方法将返回 PassThrough 流，您可以将该流传递到其他位置（或仅监听事件）。 注意：这仅在 node >= 0.10 时可用。
```
var command = ffmpeg('/path/to/file.avi')
  .videoCodec('libx264')
  .audioCodec('libmp3lame')
  .size('320x240')
  .on('error', function(err) {
    console.log('An error occurred: ' + err.message)
  })
  .on('end', function() {
    console.log('Processing finished !')
  })
var ffstream = command.pipe()
ffstream.on('data', function(chunk) {
  console.log('ffmpeg just wrote ' + chunk.length + ' bytes')
})
```
run(): 开始处理
Aliases: exec(), execute() 当产生多个输出时，此方法主要有用（否则，save()或 stream()方法更简单）。它开始处理指定的输出。 警告：在调用其他处理方法（例如 save()，pipe()或 screenshots()）时，请勿使用 run()。
```
ffmpeg('/path/to/file.avi')
  .output('screenshot.png')
  .noAudio()
  .seek('3:00')
  .output('small.avi')
  .audioCodec('copy')
  .size('320x200')
  .output('big.avi')
  .audioCodec('copy')
  .size('640x480')
  .on('error', function(err) {
    console.log('An error occurred: ' + err.message)
  })
  .on('end', function() {
    console.log('Processing finished !')
  })
  .run()
```
mergeToFile(filename, tmpdir): 连接多个输入
在命令上使用 input 和 mergeToFile 方法将多个输入连接到单个输出文件。 mergeToFile 需要一个临时文件夹作为第二个参数。
```
ffmpeg('/path/to/part1.avi')
  .input('/path/to/part2.avi')
  .input('/path/to/part2.avi')
  .on('error', function(err) {
    console.log('An error occurred: ' + err.message)
  })
  .on('end', function() {
    console.log('Merging finished !')
  })
  .mergeToFile('/path/to/merged.avi', '/path/to/tempDir')
```
screenshots(options[, dirname]): 产生缩图
Aliases: thumbnail(), thumbnails(), screenshot(), takeScreenshots() 使用screenshots方法提取一个或多个缩略图，并将其另存为 PNG 文件。但是，此实现有一些注意事项：

在输入流上不起作用
进度事件报告的进度信息不准确
它与过滤器的交互效果不佳。特别是，不要使用 size() 方法来调整缩略图的大小，而应使用 size option 代替。
options 参数是具有以下键的对象：

folder: 生成图像文件的输出文件夹。默认为当前文件夹。
filename: 输出文件名模式（请参见下文）。默认为“tn.png”。
count: 指定要生成多少个缩略图。使用此选项时，视频中会定期生成缩略图（例如，当请求 3 个缩略图时，分别为视频长度的 25％，50％和 75％）。指定时间戳或时间戳时，将忽略 count。
timemarks or timestamps: 指定应在其中拍摄缩略图的视频中的时间戳数组。每个时间戳可能是一个数字（以秒为单位），百分比字符串（例如“ 50％”）或格式为“hh:mm:ss.xxx”的时间戳字符串（小时，分钟和毫秒均为可选）。
size: 指定缩略图的目标大小（与.size()方法具有相同的格式）。注意：生成缩略图时，不应使用.size()方法。
filename选项为生成的文件指定文件名模式。它可能包含以下格式标记：

‘%s’: 偏移量（以秒为单位）
‘%w’: 屏幕截图宽度
‘%h’: 屏幕截图高度
‘%r’: 屏幕截图分辨率（与'％wx％h’相同）
‘%f’: 输入文件名
‘%b’: 输入基本名称（不带扩展名的文件名）
‘%i’: 屏幕快照在时间标记数组中的索引（可以像％000i 这样使用它来补零）
如果传递了多个时间戳，并且在文件名模式中未指定任何可变格式标记（'％s’或'％i'），则将自动添加_％i。 生成缩略图时，将使用生成的文件名数组作为参数调度其他文件名事件。
```
ffmpeg('/path/to/video.avi')
  .on('filenames', function(filenames) {
    console.log('Will generate ' + filenames.join(', '))
  })
  .on('end', function() {
    console.log('Screenshots taken')
  })
  .screenshots({
    // Will take screens at 20%, 40%, 60% and 80% of the video
    count: 4,
    folder: '/path/to/output'
  })
ffmpeg('/path/to/video.avi').screenshots({
  timestamps: [30.5, '50%', '01:10.123'],
  filename: 'thumbnail-at-%s-seconds.png',
  folder: '/path/to/output',
  size: '320x240'
})
```
Controlling the FFmpeg process
kill([signal=‘SIGKILL’]): 杀死任何正在运行的 ffmpeg 进程
此方法将 signal（默认为“SIGKILL”）发送到 ffmpeg 进程。只有在处理开始时才有意义。发送终止过程的信号将导致发出 error 事件。
```
var command = ffmpeg('/path/to/video.avi')
  .videoCodec('libx264')
  .audioCodec('libmp3lame')
  .on('start', function() {
    // Send SIGSTOP to suspend ffmpeg
    command.kill('SIGSTOP')
    doSomething(function() {
      // Send SIGCONT to resume ffmpeg
      command.kill('SIGCONT')
    })
  })
  .save('/path/to/output.mp4')
// Kill ffmpeg after 60 seconds anyway
setTimeout(function() {
  command.on('error', function() {
    console.log('Ffmpeg has been killed')
  })
  command.kill()
}, 60000)
```
renice([niceness=0]): 更改 ffmpeg 进程优先级
此方法更改任何正在运行的 ffmpeg 进程（如果有）以及将来产生的任何进程的 niceness（优先级）值。niceness参数的范围可以从-20（最高优先级）到 20（最低优先级），并且默认值为 0（这是大多数*nix 系统上的默认过程友好度）。 注意：此方法在 Windows 平台上无效。
```
// Set startup niceness
var command = ffmpeg('/path/to/file.avi')
  .renice(5)
  .save('/path/to/output.mp4')
// Command takes too long, raise its priority
setTimeout(function() {
  command.renice(-5)
}, 60000)
```
Reading video metadata
您可以使用模块ffprobe方法从任何有效的ffmpeg输入文件中读取元数据。
```
ffmpeg.ffprobe('/path/to/file.avi', function(err, metadata) {
    console.dir(metadata);
});
```
您也可以在FfmpegCommand上调用ffprobe方法以探查其输入之一。您可以将基于0的输入数字作为第一个参数传递，以指定从哪个输入中读取元数据，否则该方法将探测最后添加的输入。
```
ffmpeg('/path/to/file1.avi')
  .input('/path/to/file2.avi')
  .ffprobe(function(err, data) {
    console.log('file2 metadata:');
    console.dir(data);
  });
ffmpeg('/path/to/file1.avi')
  .input('/path/to/file2.avi')
  .ffprobe(0, function(err, data) {
    console.log('file1 metadata:');
    console.dir(data);
  });
```
警告：ffprobe可能会与输入流一起调用，但是在这种情况下，它将消耗流中的数据，并且该数据将不再可用于ffmpeg。除非流是实时流，否则在同一输入流上同时使用ffprobe和代码转换命令很可能会失败。仅当您知道自己在做什么时才这样做。

返回的对象与从外壳程序运行以下命令所返回的对象相同（取决于ffmpeg的版本，您可能必须将-of替换为-print_format）：
```
$ ffprobe -of json -show_streams -show_format /path/to/file.avi
```
它将包含有关容器（作为format键）和流数组（作为stream键）的信息。格式对象和每个流对象还包含元数据标签，具体取决于格式：
```
{
  "streams": [
    {
      "index": 0,
      "codec_name": "h264",
      "codec_long_name": "H.264 / AVC / MPEG-4 AVC / MPEG-4 part 10",
      "profile": "Constrained Baseline",
      "codec_type": "video",
      "codec_time_base": "1/48",
      "codec_tag_string": "avc1",
      "codec_tag": "0x31637661",
      "width": 320,
      "height": 180,
      "has_b_frames": 0,
      "sample_aspect_ratio": "1:1",
      "display_aspect_ratio": "16:9",
      "pix_fmt": "yuv420p",
      "level": 13,
      "r_frame_rate": "24/1",
      "avg_frame_rate": "24/1",
      "time_base": "1/24",
      "start_pts": 0,
      "start_time": "0.000000",
      "duration_ts": 14315,
      "duration": "596.458333",
      "bit_rate": "702655",
      "nb_frames": "14315",
      "disposition": {
        "default": 0,
        "dub": 0,
        "original": 0,
        "comment": 0,
        "lyrics": 0,
        "karaoke": 0,
        "forced": 0,
        "hearing_impaired": 0,
        "visual_impaired": 0,
        "clean_effects": 0,
        "attached_pic": 0
      },
      "tags": {
        "creation_time": "1970-01-01 00:00:00",
        "language": "und",
        "handler_name": "\fVideoHandler"
      }
    },
    {
      "index": 1,
      "codec_name": "aac",
      "codec_long_name": "AAC (Advanced Audio Coding)",
      "codec_type": "audio",
      "codec_time_base": "1/48000",
      "codec_tag_string": "mp4a",
      "codec_tag": "0x6134706d",
      "sample_fmt": "fltp",
      "sample_rate": "48000",
      "channels": 2,
      "bits_per_sample": 0,
      "r_frame_rate": "0/0",
      "avg_frame_rate": "0/0",
      "time_base": "1/48000",
      "start_pts": 0,
      "start_time": "0.000000",
      "duration_ts": 28619776,
      "duration": "596.245333",
      "bit_rate": "159997",
      "nb_frames": "27949",
      "disposition": {
        "default": 0,
        "dub": 0,
        "original": 0,
        "comment": 0,
        "lyrics": 0,
        "karaoke": 0,
        "forced": 0,
        "hearing_impaired": 0,
        "visual_impaired": 0,
        "clean_effects": 0,
        "attached_pic": 0
      },
      "tags": {
        "creation_time": "1970-01-01 00:00:00",
        "language": "und",
        "handler_name": "\fSoundHandler"
      }
    }
  ],
  "format": {
    "filename": "http://download.blender.org/peach/bigbuckbunny_movies/BigBuckBunny_320x180.mp4",
    "nb_streams": 2,
    "format_name": "mov,mp4,m4a,3gp,3g2,mj2",
    "format_long_name": "QuickTime / MOV",
    "start_time": "0.000000",
    "duration": "596.459000",
    "size": "64657027",
    "bit_rate": "867211",
    "tags": {
      "major_brand": "isom",
      "minor_version": "512",
      "compatible_brands": "mp41",
      "creation_time": "1970-01-01 00:00:00",
      "title": "Big Buck Bunny",
      "artist": "Blender Foundation",
      "composer": "Blender Foundation",
      "date": "2008",
      "encoder": "Lavf52.14.0"
    }
  }
}
```
查询ffmpeg功能
fluent-ffmpeg使您可以查询已安装的ffmpeg版本以获取受支持的格式，编解码器，编码器和过滤器。
```
var Ffmpeg = require('fluent-ffmpeg');
Ffmpeg.getAvailableFormats(function(err, formats) {
  console.log('Available formats:');
  console.dir(formats);
});
Ffmpeg.getAvailableCodecs(function(err, codecs) {
  console.log('Available codecs:');
  console.dir(codecs);
});
Ffmpeg.getAvailableEncoders(function(err, encoders) {
  console.log('Available encoders:');
  console.dir(encoders);
});
Ffmpeg.getAvailableFilters(function(err, filters) {
  console.log("Available filters:");
  console.dir(filters);
});
// Those methods can also be called on commands
new Ffmpeg({ source: '/path/to/file.avi' })
  .getAvailableCodecs(...);
```
这些方法使用每种可用格式，编解码器或过滤器的键将对象传递给其回调。 返回的格式对象如下所示：
```
{
  ...
  mp4: {
    description: 'MP4 (MPEG-4 Part 14)',
    canDemux: false,
    canMux: true
  },
  ...
}
```
canDemux指示ffmpeg是否能够从该格式提取流（解复用）
canMux指示ffmpeg是否能够将流写入（mux）此格式
编解码器的返回对象如下所示：
```
{
  ...
  mp3: {
    type: 'audio',
    description: 'MP3 (MPEG audio layer 3)',
    canDecode: true,
    canEncode: true,
    intraFrameOnly: false,
    isLossy: true,
    isLossless: false
  },
  ...
}
```
type指示编解码器类型，“音频”，“视频”或“字幕”
canDecode告诉ffmpeg是否能够使用此编解码器解码流
canEncode告诉ffmpeg是否能够使用此编解码器对流进行编码
根据您的ffmpeg版本（或者如果您使用avconv），可能会出现其他键，例如：

directRendering告诉编解码器是否可以直接在GPU RAM中呈现；对于转码无用
intraFrameOnly告诉编解码器是否只能使用I-frames
isLossy告诉编解码器是否可以进行有损编码/解码
isLossless告诉编解码器是否可以进行无损编码/解码
对于某些 ffmpeg/avcodec 版本，描述中包含了以“Foo编解码器（decoders：libdecodefoo）（encoders：libencodefoo）”形式出现的编码器/解码器。在这种情况下，您将改为使用那些编码器/解码器（由getAvailableCodecs返回的编解码器对象也将包括它们）。

返回的编码器对象如下所示：
```
{
  ...
  libmp3lame: {
    type: 'audio',
    description: 'MP3 (MPEG audio layer 3) (codec mp3)',
    frameMT: false,
    sliceMT: false,
    experimental: false,
    drawHorizBand: false,
    directRendering: false
  },
  ...
}
```
type指示编码器类型，“音频”，“视频”或“字幕”
experimental指示编码器是否为实验性。使用这种编解码器时，fluent-ffmpeg会自动添加“ -strict实验”标志。
返回的过滤器对象如下所示：
```
{
  ...
  scale: {
    description: 'Scale the input video to width:height size and/or convert the image format.',
    input: 'video',
    multipleInputs: false,
    output: 'video',
    multipleOutputs: false
  },
  ...
}
```
input告诉此过滤器操作的输入类型，“音频”，“视频”或“无”之一。如果为“none”，则过滤器可能不输出任何内容
multipleInputs告诉过滤器是否可以接受多个输入
output告诉此过滤器生成的输出类型，“音频”，“视频”或“无”之一。如果为“ none”，则过滤器没有输出（仅接收器）
multipleInputs告诉过滤器是否可以生成多个输出
Cloning an FfmpegCommand
您可以通过调用clone（）方法来创建FfmpegCommand实例的副本。克隆将是被调用时原始副本的精确副本（相同的输入，相同的配置项，相同的事件处理程序等）。当您要在同一输入上应用不同的处理选项时，这主要有用。 在克隆上设置配置项，添加输入或事件处理程序不会影响原始命令。
```
// Create a command to convert source.avi to MP4
var command = ffmpeg('/path/to/source.avi')
  .audioCodec('libfaac')
  .videoCodec('libx264')
  .format('mp4');
// Create a clone to save a small resized version
command.clone()
  .size('320x200')
  .save('/path/to/output-small.mp4');
// Create a clone to save a medium resized version
command.clone()
  .size('640x400')
  .save('/path/to/output-medium.mp4');
// Save a converted version with the original size
command.save('/path/to/output-original-size.mp4');
```
