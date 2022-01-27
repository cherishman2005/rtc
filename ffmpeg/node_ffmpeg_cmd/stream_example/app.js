const express = require('express');
const fs = require('fs');
const path = require('path');
const stream = require('stream');
const axios = require('axios');
const client = require('./bce_client');
const ffmpeg = require('fluent-ffmpeg');
const ffmpegPath = '/usr/bin/ffmpeg';
const app = express();
const http = require('http');
//const Koa = require('koa');
//const app = new Koa();

const router = express.Router();

/*
let server = http.createServer(function(req, res) {
  let writer = fs.createWriteStream('test_stream.jpg') 

  req.on('data', function(chunk) {
    writer.write(chunk);
  });
  req.on('end', function() {
    //writer.end();
    
    res.write("ok");
    res.end();
  });
});

server.listen(3000);
*/



app.use(router.post('/upload/:filename', (req, res, next) => {
  console.log("params.filename:", req.params.filename);
  let filename = req.params.filename;
  console.log("headers", req.headers);
  let buffers = [];
  req.on('data',(trunk) => {
    console.info(trunk.length);
    buffers.push(trunk);
  }).on('end',async () => {
      const buffer= Buffer.concat(buffers);
      fs.writeFileSync(filename, buffer);
      return res.end();
    }).on('close', () => {
      res.status(400).json({"err_code":"40000"});
    }).on('error', () => {
      res.status(400).json({"err_code":"40000"});
    });
}))
app.listen(3000);