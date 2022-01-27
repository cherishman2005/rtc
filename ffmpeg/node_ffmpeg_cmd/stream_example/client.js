const http = require('http');
const fs = require('fs');
const path = require('path');

let option = {
          host:"127.0.0.1",   //请求host
          path:"/uploadFile",  //请求链接
          port:3000,            //端口
          method: "POST",  //请求类型
          headers:{   //请求头
            
                'Content-Type': 'application/octet-stream',  //数据格式为二进制数据流
                'Transfer-Encoding': 'chunked',  //传输方式为分片传输
                'Connection': 'keep-alive'    //这个比较重要为保持链接。
          }
      }
      
let req  = http.request(option, res => {
  console.log(`statusCode: ${res.statusCode}`)

  res.on('data', d => {
    process.stdout.write(d)
  })
});

req.on('error', error => {
  console.error(error)
})

fs.createReadStream(path.join(__dirname,"snapshot-01.png"))
  .on("open",chunk=>{
  })
  .on("data",chunk=>{
      req.write(chunk);  //发送数据
  })
  .on("end",()=>{
     req.end();   //发送结束
  })

