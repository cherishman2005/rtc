# nginx-rtmp

## record直播录制配置

```
        application live {
            live on;
            #gop_cache on; #open GOP cache for reducing the wating time for the first picture of video
            
            record all;
            record_path /data/services/nginx/openresty/video/recordings;
            record_unique on;
        }
```
