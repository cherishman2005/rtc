// ffmpeg -i https://yy-ai-train.bj.bcebos.com/dataset/zhangbiwu/video/test.mp4 -f image2 -vframes 1 -vcodec png -f rawvideo -s 320x240 -ss 00:00:01 snapshot-01.png
const ffmpeg = require('fluent-ffmpeg');

const ffmpegPath = "/usr/bin/ffmpeg";
const inputPath = 'https://yy-ai-train.bj.bcebos.com/dataset/zhangbiwu/video/test.mp4';
let file = __dirname + '/snapshot-01.png';

var proc = ffmpeg(inputPath)
    .setFfmpegPath(ffmpegPath)
    .on('end', function(files)
    {
        //res.sendfile(file);
        console.log('end');
    })
    .on('error', function(err)
    {
        /*
        res.json({
            status : 'error',
            error : err.message
        });
        */
        console.log(err);
    })
    .outputOptions(['-f image2', '-vframes 1', '-vcodec png', '-f rawvideo', '-s 320x240', '-ss 00:02:01'])
    .output(file)
    .run();
