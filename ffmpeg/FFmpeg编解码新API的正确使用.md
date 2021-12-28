# FFmpeg编解码新API的正确使用

由于一直对FFmpeg中新的编解码API不太理解，而网上不少资料还是使用旧API，即使是使用新API的，其中一些说法亦不足以令鄙人理解，遂在官方网页上找到了相关的解释，算是比较清晰的说明新API的用法和注意事项，故本文参照其内容做一番记录。

## 基本使用方法

新的编解码API对输入输出进行了解耦，使得原来的每一个函数拆分为了一对函数。

解码：avcodec_send_packet()、avcodec_receive_frame()
编码：avcodec_send_frame()、avcodec_receive_packet()
使用的步骤：

建立并打开AVCodecContext。

发送有效的输入：

对于解码，调用avcodec_send_packet()给解码器发送压缩数据：AVPacket。
对于编码，调用avcodec_send_frame()给编码器发送原始的未压缩数据：AVFrame。

在这两种情况下，为了提升效率，均建议AVPacket和AVFrame使用引用计数的方式，否则libavcodec可能必须拷贝输入的数据(libavformat总是返回引用计数的AVPacket，而av_frame_get_buffer()分配引用计数AVFrame)。

在循环中接收输出。定期调用相应的接收函数并处理它们的输出：

对于解码，调用avcodec_receive_frame()，成功的话会拿到解压缩的数据：AVFrame
对于编码，调用avcodec_receive_packet()，成功的话会拿到压缩后的数据：AVPacket

在循环中重复调用上述函数，直到它返回AVERROR（EAGAIN）或错误。AVERROR（EAGAIN）返回值表示输入的数据已经全部处理完了，需要新的输入数据才能返回新输出，注意此次输出的frame是没有数据的。在这种情况下，需要继续发送新的输入。对于每个输入的frame/packet，编解码器通常将返回1个输出的packet/frame，但它的个数也可以是0(无数据)或大于1。比如音频情况下可能会大于1。对于0的情况，一个是上面说到的AVERROR（EAGAIN）；另一个是在解码或编码开始时，编解码器可以接受多个输入的frame/packet而不返回packet/frame，直到其内部缓冲器被填满才会返回。如果按照上述步骤操作，则此情况已经被良好地处理了。

## 流结束的情况

文件流结束后都需要“刷新”(也叫draining)编解码器，因为编解码器可能出于性能考虑或由于必须的原因(比如考虑b帧时)会在内部缓冲多个帧或包，为了拿到这些剩余的数据，处理方法如下：

文件结束时，将NULL(而不是有效的输入，具体来说比如解码时将输入的packet设置为：pkt->data = NULL;pkt->size = 0;)发送到avcodec_send_packet()(解码时)或avcodec_send_frame()(编码时)函数，这将进入draining模式。

在循环中调用avcodec_receive_frame()(解码)或avcodec_receive_packet()(编码)时，加入对返回值是否是AVERROR_EOF的判断。当输入NULL进入draining模式后将会返回AVERROR_EOF而不会返回AVERROR(EAGAIN)。

捕获AVERROR_EOF后，调用avcodec_flush_buffers()刷新编解码器使得可以恢复重新解码。

