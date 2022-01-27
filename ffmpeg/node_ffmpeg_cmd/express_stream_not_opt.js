const express = require('express');
const fs = require('fs');
const client = require('./bce_client');
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
    const videoPath = 'https://yy-ai-train.bj.bcebos.com/dataset/zhangbiwu/video/test.mp4';
    //let file = __dirname + '/snapshot-01.png';
    let paramsMap = new Map();
    paramsMap.set("snapshot-01.png", videoPath)
    
    let inputPath = paramsMap.get(req.params.filename)
    if (!inputPath) {
        res.status(404).json({
            code: 404,
            msg: "filename not found"
        })
        
        return
    }

    res.contentType('image/png');

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

async function streamToBuffer (stream) {
    return new Promise(async (resolve, reject) => {
      const data = [];

      stream.on('data', (chunk) => {
        data.push(chunk);
      });

      stream.on('end', () => {
        resolve(Buffer.concat(data))
      })

      stream.on('error', (err) => {
        reject(err)
      })
   
    })
  }

const snapshot123 = async function (req, res) {
    const videoPath = 'https://yy-ai-train.bj.bcebos.com/dataset/zhangbiwu/video/test.mp4';
    //let file = __dirname + '/snapshot-01.png';
    let paramsMap = new Map();
    paramsMap.set("2022", videoPath)
    
    let inputPath = paramsMap.get(req.params.filename)
    if (!inputPath) {
        res.status(404).json({
            code: 404,
            msg: "filename not found"
        })
        
        return
    }

    //res.contentType('image/png');
    let tmp = `/tmp/${req.params.filename}.png`;
    let stream = fs.createReadStream(tmp);
    let proc = ffmpeg(inputPath)
        .setFfmpegPath(ffmpegPath)
        .on('end', function(files)
        {
            //res.sendfile(file);
            console.log('end: typeof stream =', typeof stream);
            //let bf = fs.readFileSync(tmp);
            //const buffer = Buffer.from(bf); 
            //console.log("bf:", bf);
            //console.log("stream:", stream);
            console.log("stream size:", fs.statSync(tmp).size)

            client.putObject(`dataset/zhangbiwu/${req.params.filename}.png`, stream, {
                "Content-Length": fs.statSync(tmp).size,
                "Content-Type": "image/png"
            })
            res.json({
                code:200,
                msg: "ok"
            });
        })
        .on('error', function(err)
        {
            res.json({
                code : 500,
                error : err.message
            });
            console.log(err);
        })
        .outputOptions(['-f image2', '-vframes 1', '-vcodec png', '-f rawvideo', '-s 320x240', '-ss 00:02:01'])
        //.output(file)
        //.run();
        // save to stream
        .pipe(stream, {end:true});

}

app.get('/video/:filename', videoStream);
app.get('/snapshot/:filename', snapshot);
app.get('/snapshot123/:filename', snapshot123);
app.set('x-powered-by', false);

app.listen(4000);