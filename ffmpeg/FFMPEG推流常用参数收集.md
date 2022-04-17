# FFMPEG推流常用参数收集

示例 
```
ffmpeg -re -stream_loop -1 -i wKgBBV7MtiaAbHhFAzl39Emwz1I432.mp4 -vf scale=1440:-2 -f flv -vcodec h264 -max_delay 1000 -g 2 rtmp://192.168.1.5/live//test
```

-re : 原始帧率

-stream_loop: 循环次数 ,-1 无限循环

-i: 源文件

-vf scale=14440:-2    : 修改分辨率,宽1440,高等比缩放

-vcodec h264: 修改编码

-max_delay 1000: 最大延迟

-g 2: gop_cache 单位

rtmp://192.168.1.5/live//test   推流地址

 

-ss表示开始切割的时间，-t表示要切多少。上面就是从15秒开始，切5秒钟出来。

 

码率控制

码率控制对于在线视频比较重要。因为在线视频需要考虑其能提供的带宽。

那么，什么是码率？很简单： 
bitrate = file size / duration 
比如一个文件20.8M，时长1分钟，那么，码率就是： 
biterate = 20.8M bit/60s = 20.8*1024*1024*8 bit/60s= 2831Kbps 
一般音频的码率只有固定几种，比如是128Kbps， 
那么，video的就是 
video biterate = 2831Kbps -128Kbps = 2703Kbps。

那么ffmpeg如何控制码率。 
ffmpg控制码率有3种选择，-minrate -b:v -maxrate 
-b:v主要是控制平均码率。 
比如一个视频源的码率太高了，有10Mbps，文件太大，想把文件弄小一点，但是又不破坏分辨率。 

```
ffmpeg -i input.mp4 -b:v 2000k output.mp4
```

上面把码率从原码率转成2Mbps码率，这样其实也间接让文件变小了。目测接近一半。 
不过，ffmpeg官方wiki比较建议，设置b:v时，同时加上 -bufsize 
-bufsize 用于设置码率控制缓冲器的大小，设置的好处是，让整体的码率更趋近于希望的值，减少波动。（简单来说，比如1 2的平均值是1.5， 1.49 1.51 也是1.5, 当然是第二种比较好） 

```
ffmpeg -i input.mp4 -b:v 2000k -bufsize 2000k output.mp4
```

-minrate -maxrate就简单了，在线视频有时候，希望码率波动，不要超过一个阈值，可以设置maxrate。 

```
ffmpeg -i input.mp4 -b:v 2000k -bufsize 2000k -maxrate 2500k output.mp4
```

Scale filter的优点是可以使用一些额外的参数
	Scale=width:height[:interl={1|-1}]
	
下面两条命令有相同效果
	ffmpeg -i input.mpg -s 320x240 output.mp4 
	ffmpeg -i input.mpg -vf scale=320:240 output.mp4
 
对输入视频成比例缩放
改变为源视频一半大小
	ffmpeg -i input.mpg -vf scale=iw/2:ih/2 output.mp4
改变为原视频的90%大小：
	ffmpeg -i input.mpg -vf scale=iw*0.9:ih*0.9 output.mp4
参考 : http://blog.7cuu.com/aid/381.html

https://blog.csdn.net/yu540135101/article/details/84346505

https://blog.csdn.net/leixiaohua1020/article/details/12751349
