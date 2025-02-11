# RTMP

## SRS-librtmp

编译SRS时，会自动编译srs-librtmp，譬如：
```
./configure --disable-all --with-librtmp && make 
```
或
```
./configure --with-librtmp --without-ssl
```

编译会生成srs-librtmp和对应的实例。

备注：支持librtmp只需要打开--with-librtmp，但推荐打开--without-ssl，不依赖于ssl，对于一般客户端（不需要模拟flash）足够了。这样srs-librtmp不依赖于任何其他库，在x86/x64/arm等平台都可以编译和运行

备注：就算打开了--with-ssl，srslibrtmp也只提供simple_handshake函数，不提供complex_handshake函数。所以推荐关闭ssl，不依赖于ssl，没有实际的用处。

SRS编译成功后，用户就可以使用这些库开发

# FAQ

## 是应该以 anxeb(startcode 0001 + nalu) 还是 avcc(len + nalu)格式 封装nalu，然后rtmp推流？

在推流到RTMP服务器时，应该使用avcc(len + nalu)格式封装nalu。avcc是H.264标准中定义的格式，包含了NALU的长度信息，便于解析和处理。使用avcc格式可以确保RTMP服务器正常接收并解析视频数据。

# 参考链接

- [https://github.com/ossrs/srs-librtmp/issues/32](https://github.com/ossrs/srs-librtmp/issues/32)
- [SRS提供的librtmp](https://blog.csdn.net/ai2000ai/article/details/78329039)
- [https://github.com/begeekmyfriend/srs-librtmp](https://github.com/begeekmyfriend/srs-librtmp)
