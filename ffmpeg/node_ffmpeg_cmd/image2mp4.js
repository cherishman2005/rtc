var ffmpeg = require('fluent-ffmpeg');

// make sure you set the correct path to your video file
var proc = ffmpeg('/data/services/nodejs/video/test_images/test-100.jpg')
  // loop for 5 seconds
  .loop(5)
  // using 25 fps
  .fps(25)
  // setup event handlers
  .on('end', function() {
    console.log('file has been converted succesfully');
  })
  .on('error', function(err) {
    console.log('an error happened: ' + err.message);
  })
  // save to file
  .save('./your_target.m4v');
