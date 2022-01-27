const express = require('express');
const fs = require('fs');
const path = require('path');
const stream = require('stream');
const axios = require('axios');
const client = require('./bce_client');
const ffmpeg = require('fluent-ffmpeg');
const ffmpegPath = '/usr/bin/ffmpeg';
//const app = express();
const http = require('http');
const Koa = require('koa');
const app = new Koa();

const router = express.Router();
/*
app.use(router.post("/uploadFile",(ctx,next)=>{
    //let data = fs.createReadStream(ctx)
       //ctx.req.setEncoding="binary";
       let url=path.join(__dirname,"/test/");
       let file = path.join(url,"node.tar.gz")
       let chunks='';
       
         ctx.req.on("data",function(d){
            let da = d.toString("utf8")
            debugger
         }).on("close",function(e){
            debugger
         }).on("error",function(){
             debugger
         }).on("end",function(){
             debugger
         })
        
        ctx.body = 'Hello World';
       next();
}))

http.createServer(app.callback()).listen(3000);
*/


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