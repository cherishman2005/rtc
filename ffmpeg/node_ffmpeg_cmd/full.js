const ffmpeg = require('fluent-ffmpeg');
const ffmpegPath = '/usr/bin/ffmpeg';
const inputPath = __dirname + '/vtest.avi';
const outputPath = __dirname + '/out_vtest.avi';

// make sure you set the correct path to your video file
let proc = ffmpeg(inputPath)
  .setFfmpegPath(ffmpegPath)
  // set video bitrate
  .videoBitrate(1024)
  // set target codec
  .videoCodec('divx')
  // set aspect ratio
  .aspect('16:9')
  // set size in percent
  .size('50%')
  // set fps
  .fps(24)
  // set audio bitrate
  .audioBitrate('128k')
  // set audio codec
  .audioCodec('libmp3lame')
  // set number of audio channels
  .audioChannels(2)
  // set custom option
  .addOption('-vtag', 'DIVX')
  // set output format to force
  .format('avi')
  // setup event handlers
  .on('end', function() {
    console.log('file has been converted succesfully');
  })
  .on('error', function(err) {
    console.log('an error happened: ' + err.message);
  })
  // save to file
  .save(outputPath);