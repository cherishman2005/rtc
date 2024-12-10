# rtp-h264

rtp组包h264

3种模式：
* single NALU
* 非交错模式  分包
* 交错模式    分包

## h264

![e20abee65f898051d29ac252a565bc1](https://github.com/user-attachments/assets/e1e7d487-40ad-4a4b-94d0-51280d71909a)


# FAQ

## packetization-mode:

表示支持的封包模式
* 当 packetization-mode 的值为 0 时或不存在时, 必须使用单一 NALU 单元模式.
* 当 packetization-mode 的值为 1 时必须使用非交错(non-interleaved)封包模式.
* 当 packetization-mode 的值为 2 时必须使用交错(interleaved)封包模式.

这个参数不可以取其他的值.

## h264 nalu rtp封装时是否要带上 start code？

在进行 H.264 NALU 到 RTP 封装时，通常情况下不需要在每个 NALU 前加上起始码（start code）。起始码在 H.264 视频码流中用于标识每个 NALU 的起始位置，但在 RTP 封装中，通常会根据不同的封装方式来区分不同的 NALU 单元，而不是使用起始码。

RTP 封装 H.264 NALU 时一般使用 Annex B 规范中的免转义字节流（即每个 NALU 前加上起始码 0x00000001，或者 0x000001）。在免转义字节流中，每个 NALU 单元之间通过起始码进行分隔，而不需要在每个 NALU 单元前单独加上起始码。
