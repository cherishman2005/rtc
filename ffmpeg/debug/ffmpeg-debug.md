# ffmpeg c++ debug

##  pts非单调递增 [libx264 @ 0x43a6540] non-strictly-monotonic PTS

```
send_frame_to_filters input:#0 codec_type:0, decoded_frame:0x4ee6100, video_size:512x288, ist->decoded_frame->pts:38038
do_video_out imwrite video_size:512x288, frame->pts:38038, imgname:./imgtmp/38038-48737.jpg, success:1, nb_frames:0
do_video_out imwrite video_size:512x288, frame->pts:38038, imgname:./imgtmp/38038-48738.jpg, success:1, nb_frames:0
[libx264 @ 0x43a6540] non-strictly-monotonic PTS
send_frame_to_filters input:#0 codec_type:0, decoded_frame:0x4ee6100, video_size:512x288, ist->decoded_frame->pts:38040
do_video_out imwrite video_size:512x288, frame->pts:38040, imgname:./imgtmp/38040-48739.jpg, success:1, nb_frames:0
do_video_out imwrite video_size:512x288, frame->pts:38040, imgname:./imgtmp/38040-48740.jpg, success:1, nb_frames:0
[libx264 @ 0x43a6540] non-strictly-monotonic PTS
do_video_out imwrite video_size:512x288, frame->pts:38040, imgname:./imgtmp/38040-48741.jpg, success:1, nb_frames:0
[libx264 @ 0x43a6540] non-strictly-monotonic PTS
send_frame_to_filters input:#0 codec_type:0, decoded_frame:0x4ee6100, video_size:512x288, ist->decoded_frame->pts:38042
do_video_out imwrite video_size:512x288, frame->pts:38042, imgname:./imgtmp/38042-48742.jpg, success:1, nb_frames:0
do_video_out imwrite video_size:512x288, frame->pts:38042, imgname:./imgtmp/38042-48743.jpg, success:1, nb_frames:0
[libx264 @ 0x43a6540] non-strictly-monotonic PTS
send_frame_to_filters input:#0 codec_type:0, decoded_frame:0x4ee6100, video_size:512x288, ist->decoded_frame->pts:38044
do_video_out imwrite video_size:512x288, frame->pts:38044, imgname:./imgtmp/38044-48744.jpg, success:1, nb_frames:0
do_video_out imwrite video_size:512x288, frame->pts:38044, imgname:./imgtmp/38044-48745.jpg, success:1, nb_frames:0
[libx264 @ 0x43a6540] non-strictly-monotonic PTS
do_video_out imwrite video_size:512x288, frame->pts:38044, imgname:./imgtmp/38044-48746.jpg, success:1, nb_frames:0
[libx264 @ 0x43a6540] non-strictly-monotonic PTS
send_frame_to_filters input:#0 codec_type:0, decoded_frame:0x4ee6100, video_size:512x288, ist->decoded_frame->pts:38046
do_video_out imwrite video_size:512x288, frame->pts:38046, imgname:./imgtmp/38046-48747.jpg, success:1, nb_frames:0
do_video_out imwrite video_size:512x288, frame->pts:38046, imgname:./imgtmp/38046-48748.jpg, success:1, nb_frames:0
[libx264 @ 0x43a6540] non-strictly-monotonic PTS
send_frame_to_filters input:#0 codec_type:0, decoded_frame:0x4ee6100, video_size:512x288, ist->decoded_frame->pts:38048
do_video_out imwrite video_size:512x288, frame->pts:38048, imgname:./imgtmp/38048-48749.jpg, success:1, nb_frames:0
do_video_out imwrite video_size:512x288, frame->pts:38048, imgname:./imgtmp/38048-48750.jpg, success:1, nb_frames:0
[libx264 @ 0x43a6540] non-strictly-monotonic PTS
send_frame_to_filters input:#0 codec_type:0, decoded_frame:0x4ee6100, video_size:512x288, ist->decoded_frame->pts:38050
do_video_out imwrite video_size:512x288, frame->pts:38050, imgname:./imgtmp/38050-48751.jpg, success:1, nb_frames:0
send_frame_to_filters input:#0 codec_type:0, decoded_frame:0x4ee6100, video_size:512x288, ist->decoded_frame->pts:38052
do_video_out imwrite video_size:512x288, frame->pts:38052, imgname:./imgtmp/38052-48752.jpg, success:1, nb_frames:0
send_frame_to_filters input:#0 codec_type:0, decoded_frame:0x4ee6100, video_size:512x288, ist->decoded_frame->pts:38054
do_video_out imwrite video_size:512x288, frame->pts:38054, imgname:./imgtmp/38054-48753.jpg, success:1, nb_frames:0
send_frame_to_filters input:#0 codec_type:0, decoded_frame:0x4ee6100, video_size:512x288, ist->decoded_frame->pts:38056
do_video_out imwrite video_size:512x288, frame->pts:38056, imgname:./imgtmp/38056-48754.jpg, success:1, nb_frames:0
send_frame_to_filters input:#0 codec_type:0, decoded_frame:0x4ee6100, video_size:512x288, ist->decoded_frame->pts:38058
do_video_out imwrite video_size:512x288, frame->pts:38058, imgname:./imgtmp/38058-48755.jpg, success:1, nb_frames:0
send_frame_to_filters input:#0 codec_type:0, decoded_frame:0x4ee6100, video_size:512x288, ist->decoded_frame->pts:38060
do_video_out imwrite video_size:512x288, frame->pts:38060, imgname:./imgtmp/38060-48756.jpg, success:1, nb_frames:0
send_frame_to_filters input:#0 codec_type:0, decoded_frame:0x4ee6100, video_size:512x288, ist->decoded_frame->pts:38062
do_video_out imwrite video_size:512x288, frame->pts:38062, imgname:./imgtmp/38062-48757.jpg, success:1, nb_frames:0
send_frame_to_filters input:#0 codec_type:0, decoded_frame:0x4ee6100, video_size:512x288, ist->decoded_frame->pts:38064
do_video_out imwrite video_size:512x288, frame->pts:38064, imgname:./imgtmp/38064-48758.jpg, success:1, nb_frames:0
send_frame_to_filters input:#0 codec_type:0, decoded_frame:0x4ee6100, video_size:512x288, ist->decoded_frame->pts:38066
do_video_out imwrite video_size:512x288, frame->pts:38066, imgname:./imgtmp/38066-48759.jpg, success:1, nb_frames:0
send_frame_to_filters input:#0 codec_type:0, decoded_frame:0x4ee6100, video_size:512x288, ist->decoded_frame->pts:38068
do_video_out imwrite video_size:512x288, frame->pts:38068, imgname:./imgtmp/38068-48760.jpg, success:1, nb_frames:0
send_frame_to_filters input:#0 codec_type:0, decoded_frame:0x4ee6100, video_size:512x288, ist->decoded_frame->pts:38070
do_video_out imwrite video_size:512x288, frame->pts:38070, imgname:./imgtmp/38070-48761.jpg, success:1, nb_frames:0
send_frame_to_filters input:#0 codec_type:0, decoded_frame:0x4ee6100, video_size:512x288, ist->decoded_frame->pts:38072
do_video_out imwrite video_size:512x288, frame->pts:38072, imgname:./imgtmp/38072-48762.jpg, success:1, nb_frames:0
send_frame_to_filters input:#0 codec_type:0, decoded_frame:0x4ee6100, video_size:512x288, ist->decoded_frame->pts:38074
do_video_out imwrite video_size:512x288, frame->pts:38074, imgname:./imgtmp/38074-48763.jpg, success:1, nb_frames:0
send_frame_to_filters input:#0 codec_type:0, decoded_frame:0x4ee6100, video_size:512x288, ist->decoded_frame->pts:38076
do_video_out imwrite video_size:512x288, frame->pts:38076, imgname:./imgtmp/38076-48764.jpg, success:1, nb_frames:0
```

## 原生ffmpeg pts单调递增


![image](https://github.com/user-attachments/assets/b2ba18b4-2347-40b9-b2b8-aab34e7eb4c0)

* 查看ffmpeg源码，ffmpeg应该有去重机制。怎么做的去重？
```
    if (print_prefix && (flags & AV_LOG_SKIP_REPEATED) && !strcmp(line, prev) &&
        *line && line[strlen(line) - 1] != '\r'){
        count++;
        if (is_atty == 1)
            fprintf(stderr, "    Last message repeated %d times\r", count);
        goto end;
    }
    if (count > 0) {
        fprintf(stderr, "    Last message repeated %d times\n", count);
        count = 0;
    }
```

# 原因

* pts非严格单调的原因是 重复对同一个视频帧进行编码

![image](https://github.com/user-attachments/assets/a15114db-b072-42f1-be49-667a666bcb70)
