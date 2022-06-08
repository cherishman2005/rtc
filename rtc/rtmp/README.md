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


# FAQ

## rtmp压测工具

- [https://github.com/ossrs/srs-bench](https://github.com/ossrs/srs-bench)
