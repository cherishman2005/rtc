# RTMP

# nginx-rtmp


**编译**

```
./configure --prefix=/data/services/nginx/openresty --with-debug --with-http_v2_module --with-stream \
            --with-cc-opt="-g -O2" \
            --with-openssl="../openssl-1.0.2k" \
            --add-module="./add_modules/nginx-http-flv-module-master"

make -j32 &&  make install

exit 0
```


**nginx.conf配置**

```
rtmp_auto_push on;
rtmp_auto_push_reconnect 1s;
rtmp_socket_dir /tmp;

rtmp {
    out_queue           4096;
    out_cork            8;
    max_streams         128;
    timeout             15s;
    drop_idle_publisher 15s;

    log_interval 5s; #interval used by log module to log in access.log, it is very useful for debug
    log_size     1m; #buffer size used by log module to log in access.log

    server {
        listen 1935;
        server_name service-test.**.com; #for suffix wildcard matching of virtual host name

        application live {
            live on;
            #gop_cache on; #open GOP cache for reducing the wating time for the first picture of video
        }

        application hls {
            live on;
            hls on;
            hls_path /tmp/hls;
        }

        application dash {
            live on;
            dash on;
            dash_path /tmp/dash;
        }
    }
}
```
# srs支持h265

1.vim src/app/srs_app_source.cpp

在这个函数中 srs_error_t SrsSource::on_video(SrsCommonMessage* shared_video)
找到if (!SrsFlvVideo::acceptable(shared_video->payload, shared_video->size))，跳转到SrsFlvVideo::acceptable（）函数定义中，下面修改内容如下：
```
bool SrsFlvVideo::acceptable(char* data, int size)
{
    // 1bytes required.
    if (size < 1) 
    {
        return false;
    }
    char frame_type = data[0];
    char codec_id = frame_type & 0x0f;
    frame_type = (frame_type >> 4) & 0x0f;
    if (frame_type < 1 || frame_type > 5) //   1<= x <= 5
    {
        return false;
    }
    /* 2020-03-31 头部问题以解决*/
    if (codec_id < 2 || codec_id > 12) {   //  2<= x <=7
        return false;
    }
    return true;
}
```

2. vim src/app/srs_app_source.cpp

这个函数中 srs_error_t SrsSource::on_video_imp(SrsSharedPtrMessage* msg) 找到
bool is_sequence_header = SrsFlvVideo::sh(msg->payload, msg->size) 这一行，跳转到sh()函数定义中
```
bool SrsFlvVideo::sh(char* data, int size)
{ //判断是否是h264
    if (h264(data, size)) 
    {
        char frame_type = data[0];
        frame_type = (frame_type >> 4) & 0x0F;
        char avc_packet_type = data[1];
        //srs_trace("h264**** data[0]=%#x,data[1]=%#x,%d,%d",data[0],data[1],frame_type, avc_packet_type );
        return frame_type == SrsVideoAvcFrameTypeKeyFrame               //1
            && avc_packet_type == SrsVideoAvcFrameTraitSequenceHeader;  //0
    }
    //判断是否是h265
    if(h265(data, size))
    {
        char frame_type = data[0];
        frame_type = (frame_type >> 4) & 0x0F;
        char avc_packet_type = data[1];
        //srs_trace("h265#### data[0]=%#x,data[1]=%#x,%d,%d",data[0],data[1],frame_type, avc_packet_type );
        return frame_type == SrsVideoHevcFrameTypeKeyFrame          //1
        && avc_packet_type == SrsVideoHevcFrameTraitSequenceHeader; //0
    }
    if (size < 2) 
    {
        return false;
    }
    return true;
}
//别忘了函数定义在.h中自行添加
bool SrsFlvVideo::h265(char *data, int size)
{
    if(size < 1)
        return false;

    char codec_id = data[0];
    codec_id = codec_id & 0x0F;

    return codec_id == SrsVideoCodecIdHEVC;
}
```

srs服务器端支持h.265编码已修改完成，重新编译srs代码
```
./configure && make && make install
```

启动服务器: 
```
./objs/srs -c conf/srs.conf
```

查看服务器是否启动成功：
```
ps -ef | grep srs
```

# FAQ

## rtmp压测工具

- [https://github.com/ossrs/srs-bench](https://github.com/ossrs/srs-bench)
- [ffmpeg支持h265](https://github.com/runner365/ffmpeg_rtmp_h265)
- [librtmp支持h265](https://blog.csdn.net/qq_33795447/article/details/89457581)
- [https://github.com/ossrs/srs/pull/1721](https://github.com/ossrs/srs/pull/1721)
- [修改srs服务器端支持h.265编码(rtmp协议)](https://blog.csdn.net/qq_44895902/article/details/106030403)
