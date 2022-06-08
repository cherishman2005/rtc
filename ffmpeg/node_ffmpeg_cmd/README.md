
# node-ffmpeg

## snapshot

start snapshot-server:
```
node express_stream.js
```

## mp4-to-push-rtmp-stream

rtmp推流(带上logo):

```
const ffmpeg = require('fluent-ffmpeg');

const ffmpegPath = "/usr/bin/ffmpeg";
const inputPath = './vtest.mp4';
const logo = './logo.png';  //水印
const outputPath = 'rtmp://seaweedfs-test.**.com/live/zhangbiwu';


var command = ffmpeg(inputPath)
    .setFfmpegPath(ffmpegPath)
    .inputOptions('-re')
    .inputOptions('-ac 2')
    .addInput(logo)
    .complexFilter([{
        filter: 'scale',
        options: [600, -1],
        inputs: '[0:v]',
        outputs: 'video'
    }, {
        filter: 'scale',
        options: [60, -1],
        inputs: '[1:v]',
        outputs: 'logo'
    }, {
        filter: 'overlay',
        options: {
            x: 'main_w-overlay_w-15',
            y: 15
        },
        inputs: ['video', 'logo']
    }])
    .on('start', function (commandLine) {
        console.log('[' + new Date() + '] Vedio is Pushing !');
        console.log('commandLine: ' + commandLine);
    })
    .on('error', function (err, stdout, stderr) {
        console.log('error: ' + err.message);
        console.log('stdout: ' + stdout);
        console.log('stderr: ' + stderr);
    })
    .on('end', function () {
        console.log('[' + new Date() + '] Vedio Pushing is Finished !');
    })
    .addOptions([
        '-vcodec libx264',
        '-preset veryfast',
        '-crf 22',
        '-maxrate 1000k',
        '-bufsize 3000k',
        '-acodec libmp3lame',
        '-ac 2',
        '-ar 44100',
        '-b:a 96k'
    ])
    .format('flv');
command
    .output(outputPath, {
        end: true
    })
    .run();
```

播放示例：

![image](https://user-images.githubusercontent.com/17688273/148151872-db6b0adc-081e-4fb3-a93d-438c8e0cf21e.png)

## 请求测试

```
time curl  http://localhost:4000/images/snapshot-01.png -o snapshot-01.png -v
```

运行结果：
```
  % Total    % Received % Xferd  Average Speed   Time    Time     Time  Current
                                 Dload  Upload   Total   Spent    Left  Speed
  0     0    0     0    0     0      0      0 --:--:-- --:--:-- --:--:--     0*   Trying 127.0.0.1...
* Connected to localhost (127.0.0.1) port 4000 (#0)
> GET /images/snapshot-01.png HTTP/1.1
> Host: localhost:4000
> User-Agent: curl/7.47.0
> Accept: */*
> 
  0     0    0     0    0     0      0      0 --:--:--  0:00:03 --:--:--     0< HTTP/1.1 200 OK
< X-Powered-By: Express
< Content-Type: image/png
< Date: Sun, 23 Jan 2022 13:43:44 GMT
< Connection: keep-alive
< Transfer-Encoding: chunked
< 
{ [16228 bytes data]
100  169k    0  169k    0     0  50049      0 --:--:--  0:00:03 --:--:-- 50052
* Connection #0 to host localhost left intact

real    0m3.474s
user    0m0.004s
sys     0m0.000s
```

# FAQ

## 怎样隐藏响应头 X-Powered-By: Express

**nodejs-express设置**
```
app.set('x-powered-by', false);
```

# 参考链接

- [https://www.psvmc.cn/article/2019-08-25-fluent-ffmpeg-rtmp.html](https://www.psvmc.cn/article/2019-08-25-fluent-ffmpeg-rtmp.html)

- [https://luneshao.github.io/2020/2020-04-07-fluent-ffmpeg-api/](https://luneshao.github.io/2020/2020-04-07-fluent-ffmpeg-api/)

- [https://github.com/fluent-ffmpeg/node-fluent-ffmpeg/issues/1106](https://github.com/fluent-ffmpeg/node-fluent-ffmpeg/issues/1106)

- [https://github.com/glynnbird/ffmpegrunner](https://github.com/glynnbird/ffmpegrunner)

- [抽帧截图](http://www.blogketori.com/wordpress/2019/08/25/nodejs%E5%AF%B9-fluent-ffmpeg%E7%BB%84%E4%BB%B6%E5%AF%B9%E8%A7%86%E9%A2%91%E8%BF%9B%E8%A1%8C%E5%88%86%E7%89%87%E5%92%8C%E5%8A%A0%E5%AF%86/)
