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
const router = express.Router();
const bodyParser = require('body-parser');

const upload = (req, res, next) => {
  console.log("params.filename:", req.params.filename);
  let filename = req.params.filename;
  console.log("headers", req.headers);
  let buffers = [];
  req.on('data',(trunk) => {
    console.info(trunk.length);
    buffers.push(trunk);
  }).on('end',async () => {
      const buffer = Buffer.concat(buffers);
      //fs.writeFileSync(filename, buffer);
      
      let objectName = `dataset/zhangbiwu/${req.params.filename}.jpeg`
      client.putObject(objectName, buffer, {
          "Content-Type": "image/jpeg"
      })
      
      let url = client.generatePresignedUrl(objectName)
      console.log("url:", url);
      return res.status(200).json({
          code: 200,
          msg: "ok",
          url: url,
      });
    }).on('close', () => {
      res.status(400).json({"err_code":"40000"});
    }).on('error', () => {
      res.status(400).json({"err_code":"40000"});
    });
}

app.post('/upload/:filename', upload);
app.set('x-powered-by', false);

app.listen(3000);
