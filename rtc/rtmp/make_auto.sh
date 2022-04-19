./configure --prefix=/data/services/nginx/openresty --with-debug --with-http_v2_module --with-stream \
            --with-cc-opt="-g -O2" \
            --with-openssl="../openssl-1.0.2k" \
            --add-module="./add_modules/nginx-http-flv-module-master" 

make -j32 &&  make install

exit 0

