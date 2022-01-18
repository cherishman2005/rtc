var ffmpeg = require('fluent-ffmpeg');

var proc = ffmpeg('vtest.avi')
    .on('filenames', function (filenames) {
       //filenames为截图保存的图片名,格式在保存的时候定义
        console.log('Will generate ' + filenames.join(', '))
    })
    .on('end', function () {
    })
    .screenshots({
        //生成多少张图 
        count: 4,
       //在指定时间位置生成图片,我这里指定的是在1秒2秒3秒的位置截3张图
        timemarks:[1,2,3],  //参数是个数组
        //图片保存位置
        folder: './shot_folder',
        //保存图片的名字格式 imgname_1
        filename: '%b_%i',
        //size: `${width}x${height}`
    });