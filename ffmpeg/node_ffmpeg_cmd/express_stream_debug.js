const express = require('express');
const fs = require('fs');
const stream = require('stream');
const http = require('http');
const axios = require('axios');
const client = require('./bce_client');
const ffmpeg = require('fluent-ffmpeg');
const ffmpegPath = '/usr/bin/ffmpeg';
const app = express();


axios.defaults.timeout = 6000 // 请求最大时间
axios.defaults.headers.post['Content-Type'] = 'application/octet-stream'
const reqUrl = 'http://127.0.0.1:8080/genBosUrl';

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
    let bufferStream = new stream.PassThrough();
    let proc = ffmpeg(inputPath)
        .setFfmpegPath(ffmpegPath)
        .on('end', function(files)
        {
            //res.sendfile(file);
            console.log('end: typeof stream =', typeof stream);
            /*
            res.json({
                code:200,
                msg: "ok"
            });
            */
        })
        .on('error', function(err)
        {
            res.json({
                code : 500,
                error : err.message
            });
            console.log(err);
        })
        .outputOptions(['-f image2', '-vframes 1', '-vcodec mjpeg', '-f rawvideo', '-s 320x240', '-ss 00:02:01'])
        //.output(file)
        //.run();
        // save to stream
        .writeToStream(bufferStream);
        
        console.log("bufferStream:", bufferStream);

        // Read the passthrough stream
        const buffers = [];
        bufferStream.on('data', function (buf) {
           buffers.push(buf);
        });
        bufferStream.on('end', async function () {
          const outputBuffer = Buffer.concat(buffers);
          let objectName = `dataset/zhangbiwu/${req.params.filename}.jpeg`
          let url = client.generatePresignedUrl(objectName)
          
          try {
            const rsp = await axios.post('http://localhost:3000/uploadFile', outputBuffer);
            console.log("bos response: ", rsp.status, rsp.data);
            if (rsp && rsp.status == 200) {
                res.json({
                    code: 200,
                    msg: "ok",
                    //url: rsp.data.url,
                })
            } else {
                res.json({
                    code: 400,
                    msg: "genBosUrl failed",
                })
            }
          } catch (e) {
            console.error('axios error:', e);
            res.json({
                code: 400,
                msg: "genBosUrl failed",
            })
          }

        });
}

app.get('/video/:filename', videoStream);
app.get('/snapshot/:filename', snapshot);
app.get('/snapshot123/:filename', snapshot123);
app.set('x-powered-by', false);

app.get('/report', function(req,res) {
    
    res.statusCode = 200;
    res.setHeader('Content-type', 'application/csv');
    res.setHeader('Access-Control-Allow-Origin', '*');

    // Header to force download
    res.setHeader('Content-disposition', 'attachment; filename=Report.csv');

    
    res.write("USERID,NAME,FBID,ACCOUNT,SUBSCRIPTION,PRICE,STATE,TIMEPERIOD\n");

    for (var i = 0; i < 10; i++) {
        res.write("23,John Doe,1234,500,SUBSCRIPITON,100,ACTIVE,30\n");
    }

    res.end();
});

app.listen(4000);
