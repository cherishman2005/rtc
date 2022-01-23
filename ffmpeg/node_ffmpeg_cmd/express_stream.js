const express = require('express');
const ffmpeg = require('fluent-ffmpeg');
const ffmpegPath = '/usr/bin/ffmpeg';

const app = express();

app.get('/video/:filename', function(req, res) {
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
});

app.listen(4000);