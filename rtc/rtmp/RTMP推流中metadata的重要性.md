# RTMP推流中metadata的重要性

偶然看到一篇文章说RTMP推流中，如果不发送flv metadata，也是可以正常显示，因为接收端显示时采用H264里SPS、PPS里的参数。

于是做了实验不发送metadata，只发送SPS/PPS包、视频帧（I帧和普通帧），用VLC拉流确实可以正常显示。

请问实际RTMP使用中，这个metadata重要吗？还是说要根据拉流端是否用到metadata来决定
