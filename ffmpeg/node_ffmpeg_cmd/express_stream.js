const express = require('express');
const ffmpeg = require('fluent-ffmpeg');
const ffmpegPath = '/usr/bin/ffmpeg';
const app = express();

const videoStream = async function (req, res) {
  res.contentType('flv');
  // make sure you set the correct path to your video file storage
  const pathToMovie = '/data/services/nodejs/ffmpeg_cmd/' + req.params.filename;
  let proc = ffmpeg(pathToMovie)
    .setFfmpegPath(ffmpegPath)
    // use the 'flashvideo' preset (located in /lib/presets/flashvideo.js)
    .preset('flashvideo')
    // setup event handlers
    .on('end', function() {
      console.log('file has been converted succesfully');
    })
    .on('error', function(err) {
      console.log('an error happened: ' + err.message);
    })
    // save to stream
    .pipe(res, {end:true});
}

const snapshot = async function (req, res) {
    res.contentType('image/png');
    const inputPath = 'https://yy-ai-train.bj.bcebos.com/dataset/zhangbiwu/video/test.mp4';
    let file = __dirname + '/snapshot-01.png';

    let proc = ffmpeg(inputPath)
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
        //.output(file)
        //.run();
        // save to stream
        .pipe(res, {end:true});
}

app.get('/video/:filename', videoStream);
app.get('/images/:filename', snapshot);
app.set('x-powered-by', false);

app.listen(4000);