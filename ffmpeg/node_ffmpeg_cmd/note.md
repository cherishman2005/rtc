# note

## express-stream

（1）视频播放
vlc player
```
http://seaweedfs-test.yy.com:4000/video/test.mp4
```

（2）抽帧截图
```
curl http://seaweedfs-test.yy.com:4000/images/snapshot-01.png -o snapshot-01.png -v
```

（3）转码


（4）rtmp推流

## stream示例

stream response
```
var express = require('express');
var router = express.Router();
var fs = require('fs')

router.get('/test', function(req, res) {
  var buf = Buffer.from('./test.html');
  fs.createReadStream(buf).pipe(res);
});
```
