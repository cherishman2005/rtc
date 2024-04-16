# ffmpeg drawtext 进阶


预备知识
https://ffmpeg.org/ffmpeg-utils.html

between(x, min, max)
Return 1 if x is greater than or equal to min and lesser than or equal to max, 0 otherwise.
...
gt(x, y)
Return 1 if x is greater than y, 0 otherwise.

gte(x, y)
Return 1 if x is greater than or equal to y, 0 otherwise.

...
if(x, y)
Evaluate x, and if the result is non-zero return the result of the evaluation of y, return 0 otherwise.

if(x, y, z)
Evaluate x, and if the result is non-zero return the evaluation result of y, otherwise the evaluation result of z.
...
lt(x, y)
Return 1 if x is lesser than y, 0 otherwise.

lte(x, y)
Return 1 if x is lesser than or equal to y, 0 otherwise.

...
mod(x, y)
Compute the remainder of division of x by y.
一些加文字的例子
前5秒(包括)加文字
```
ffmpeg -i bunny.mp4 -vf drawtext=fontcolor=white:fontsize=40:fontfile=test.ttf:line_spacing=7:text='test':x=50:y=50:enable='lte(t\,5)'   -y out.mp4
```

从5秒(包括)之后加文字
```
ffmpeg -i bunny.mp4 -vf drawtext=fontcolor=white:fontsize=40:fontfile=test.ttf:line_spacing=7:text='test':x=50:y=50:enable='gte(t\,5)'   -y out.mp4
```

第10秒到20秒加文字
```
ffmpeg -i bunny.mp4 -vf drawtext=fontcolor=white:fontsize=40:fontfile=test.ttf:line_spacing=7:text='test':x=50:y=50:enable='between(t\,10\,20)'   -y out.mp4
```
切片的时候使用 比如每个切片10秒 对第二个切片的前5秒 加文字 也就是总文件的10~15秒 当前切片的前5秒
```
ffmpeg -i bunny.mp4 -vf drawtext=fontcolor=white:fontsize=40:fontfile=msyh.ttf:line_spacing=7:text='test':x=50:y=50:enable='between(t

+10\,0\,15)'   -y out.mp4
```
显示5秒 然后5秒不显示 然后再显示5秒 然后5秒不显示； 依次。。。
```
ffmpeg -i bunny.mp4 -vf drawtext=fontcolor=white:fontsize=40:fontfile=test.ttf:line_spacing=7:text='test':x=50:y=50:enable='lte(mod(t\,10)\,5)'   -y out.mp4
```
5秒显示hello 接着5秒显示world 依次
```
ffmpeg -i bunny.mp4 -vf drawtext=fontcolor=white:fontsize=40:fontfile=test.ttf:line_spacing=7:text='hello':x=50:y=50:enable='lte(mod(t

\,10)\,5)',drawtext=fontcolor=white:fontsize=40:fontfile=test.ttf:line_spacing=7:text='world':x=50:y=50:enable='gte(mod(t\,10)\,5)'   -y 

out.mp4
```

文字从左向右移动 每帧移动2个像素
```
 ffmpeg -i bunny.mp4 -vf drawtext=fontcolor=white:fontsize=40:fontfile=test.ttf:line_spacing=7:text='test':x=50+n*2:y=50  -y 
out.mp4
```

打印real time
```
 ffmpeg -i bunny.mp4 -vf drawtext=fontcolor=white:fontsize=20:fontfile=test.ttf:line_spacing=7:text='%{localtime}':x=20:y=20 -vframes 600  -y out.mp4
```

References:
http://blog.threadblocked.com/2018/06/09/post1/
