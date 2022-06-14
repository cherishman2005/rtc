# nginx-rtmp支持h265编码

## 问题背景
rtmp协议和flv媒体格式都是adobe公司标准，但是由于近些年flash的表现越来越差，adobe公司对flash和rtmp、flv也已经逐渐放弃升级。导致rtmp、flv协议停留在h264编码时代，近些年h265编码火起来之后rtmp和flv协议是无法支持它的，为了支持rtmp和flv编码国内几个CDN厂商约定将flv中codecId为12（十六进制 0x0c）时代表h265编码。

## 解决方案

在https://github.com/im-pingo/nginx-rtmp-module 中对rtmp支持了h265的编码，并且也对HLS、HTTP-FLV、HTTP-TS都进行了h265的支持。关于部署和配置请参照我以前的博客。 目前在我的开源项目中，rtmp和flv默认使用12（十六进制 0x0c）代表h265编码。同时推流工具也要配合修改，使用12做为h265的。 以ffmpeg推流工具为例，如果你需要能够推送支持h265编码的rtmp流，你需要下载特殊的ffmpeg版本，ffmpeg源码连接：https://github.com/im-pingo/CDN-Union_H265 ffmpeg编译过程可参考ffmpeg官网文档：https://trac.ffmpeg.org/wiki/CompilationGuide/Centos

ffmpeg编译成功后使用ffmpeg推流：
```
ffmpeg -i input.mp4 -vcodec h265 -acodec aac -f flv rtmp://ip/app/name
```
