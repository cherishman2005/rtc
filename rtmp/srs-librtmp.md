
# rtmp

## librtmp做Server

群里有很多人问，librtmp如何做server，实在不胜其骚扰，所以单列一章。

server的特点是会有多个客户端连接，至少有两个：一个推流连接，一个播放连接。所以server有两种策略：

每个连接一个线程或进程：像apache。这样可以用同步socket来收发数据（同步简单）。坏处就是没法支持很高并发，1000个已经到顶了，得开1000个线程/进程啊。
使用单进程，但是用异步socket：像nginx这样。好处就是能支持很高并发。坏处就是异步socket麻烦。
rtmpdump提供的librtmp，当然是基于同步socket的。所以使用librtmp做server，只能采取第一种方法，即用多线程处理多个连接。多线程多麻烦啊！要锁，同步，而且还支持不了多少个。

librtmp的定位就是客户端程序，偏偏要超越它的定位去使用，这种大约只有中国人才能这样“无所畏惧”。

嵌入式设备上做rtmp server，当然可以用srs/crtmpd/nginx-rtmp，轮也轮不到librtmp。

## SRS为何提供librtmp

srs提供的客户端srs-librtmp的定位和librtmp不一样，主要是：

librtmp的代码确实很烂，毋庸置疑，典型的代码堆积。
librtmp接口定义不良好，这个对比srs就可以看出，使用起来得看实现代码。
没有实例：接口的使用最好提供实例，srs提供了publish/play/rtmpdump实例。
最小依赖关系：srs调整了模块化，只取出了core/kernel/rtmp三个模块，其他代码没有编译到srs-librtmp中，避免了冗余。
最少依赖库：srs-librtmp只依赖c/c++标准库（若需要复杂握手需要依赖openssl，srs也编译出来了，只需要加入链接即可）。
不依赖st：srs-librtmp使用同步阻塞socket，没有使用st（st主要是服务器处理并发需要）。
SRS提供了测速函数，直接调用srs-librtmp就可以完成到服务器的测速。参考：Bandwidth Test
SRS提供了日志接口，可以获取服务器端的信息，譬如版本，对应的session id。参考：Tracable log
支持直接发布h.264裸码流，参考：publish-h264-raw-data
SRS可以直接导出一个srs-librtmp的project，编译成.h和.a使用。或者导出为.h和.cpp，一个大文件。参考：export srs librtmp
一句话，srs为何提供客户端开发库？因为rtmp客户端开发不方便，不直观，不简洁。
