#!/bin/bash

echo "Input num : "

read num

for ((i=0; i<${num}; i++));
do
	./ffmpeg -hwaccel cuvid -vcodec h264_cuvid -i '/data/services/video/win.HD.1080p.mp4' -c:v h264_nvenc -f rawvideo -y /dev/null -an > ${i}.log 2>&1 &
done
