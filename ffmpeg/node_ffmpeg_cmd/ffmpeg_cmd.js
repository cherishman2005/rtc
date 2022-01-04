var ffmpeg = require('fluent-ffmpeg');
//var command = ffmpeg();

// make sure you set the correct path to your video file
ffmpeg.ffprobe('./vtest.avi',function(err, metadata) {
  console.log(require('util').inspect(metadata, false, null));
});