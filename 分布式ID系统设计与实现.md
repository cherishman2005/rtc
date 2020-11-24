

# 分布式ID系统设计与实现

本文主要分析分布式ID系统设计，并采用nodejs + msyql技术栈快速实现了分布式ID系统。

ID发号系统（唯一ID号）几乎所有互联网系统都需要使用到，典型的应用场景：

* 电商个模块的ID；

* IM聊天系统；

如果要做到可靠性好，容灾等，需要考虑分布式ID系统。

## 事务实现ID号段分配

![leaf-ids](/img/leaf_ids.png)

```
Begin
UPDATE table SET max_id=max_id+step WHERE biz_tag=xxx
SELECT biz_tag, max_id, step FROM table WHERE biz_tag=xxx
Commit
```

优点：

* Leaf服务可以很方便的线性扩展，性能完全能够支撑大多数业务场景。
* ID号码是趋势递增的8byte的64位数字，满足上述数据库存储的主键要求。
* 容灾性高：Leaf服务内部有号段缓存，即使DB宕机，短时间内Leaf仍能正常对外提供服务。
* 可以自定义max_id的大小，非常方便业务从原有的ID方式上迁移过来。

缺点：

* ID号码不够随机，能够泄露发号数量的信息，不太安全。
* TP999数据波动大，当号段使用完之后还是会hang在更新数据库的I/O上，tg999数据会出现偶尔的尖刺。
* DB宕机会造成整个系统不可用。


【注】ID号码不够随机，能够泄露发号数量的信息，不太安全。
 —— 适用场景不一样，不能归结为缺点。如果需要ID随机的场景（如支付二维码链接中的ID号，一物一码跟踪系统等），可以针对做加密、解密等映射操作。

## ab压测

ab简单测试：
ab -n 1000 -c 50 http://127.0.0.1:9191/seqid/get
```
Server Software:        
Server Hostname:        127.0.0.1
Server Port:            9191

Document Path:          /seqid/get
Document Length:        42 bytes

Concurrency Level:      50
Time taken for tests:   2.953 seconds
Complete requests:      1000
Failed requests:        620
   (Connect: 0, Receive: 0, Length: 620, Exceptions: 0)
Total transferred:      249620 bytes
HTML transferred:       42620 bytes
Requests per second:    338.61 [#/sec] (mean)
Time per request:       147.664 [ms] (mean)
Time per request:       2.953 [ms] (mean, across all concurrent requests)
Transfer rate:          82.54 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0    0   1.1      0       7
Processing:     4  145  54.3    129     353
Waiting:        2  139  53.9    124     352
Total:          9  146  54.1    129     353

Percentage of the requests served within a certain time (ms)
  50%    129
  66%    135
  75%    147
  80%    152
  90%    200
  95%    345
  98%    349
  99%    351
 100%    353 (longest request)
```
QPS在300/s，并发请求数时延mean=2.9ms。


# 小结


# 参考链接

- [https://tech.meituan.com/2017/04/21/mt-leaf.html](https://tech.meituan.com/2017/04/21/mt-leaf.html)

- [分布式系统ID的生成方法之UUID、数据库、算法、Redis、Leaf方案](https://www.toutiao.com/i6799541297808933387/?tt_from=weixin&utm_campaign=client_share&wxshare_count=1&timestamp=1604585940&app=news_article&utm_source=weixin&utm_medium=toutiao_android&use_new_style=1&req_id=202011052218590101310752153E11CFA3&group_id=6799541297808933387)

