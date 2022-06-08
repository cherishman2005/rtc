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

