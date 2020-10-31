# ZEGO实时音视频服务架构

ZEGO流媒体服务系统架构

![ZEGO直播架构](/img/zego-live-architecture.png)

AVERTP：ZEGO的私有音视频协议。

流媒体服务包含的具体职责：

* 调度：在用户推拉流前，需要发起调度请求，获得一个资源后才能够发起实际的码流推拉。用户体验的好坏，跟调度策略有很大关系。

* 推拉流：适配RTMP，WebRTC，AVERTP等多种协议的推拉流标准，提供了更优的流控等算法。

* 转推CDN：即构全球实时网络和第三方的CDN需要进行协作来满足客户的多样化场景需求。

* 转码：RTMP使用的AAC音频编码，WebRTC使用的Opus音频编码，这两种格式互通，需要对音频进行转码。

* 转协议：RTMP和WebRTC是以网关的形式存在的，中间的网络传输都是以AVERTP协议来进行。

  目前支持H264，H265，VP8 3种格式的视频编码转码；AAC, Opus，SILK 3种格式的音频转码；RTMP、WebRTC、AVERTP3种协议格式的转协议。

* 混流：当出现多个主播连麦互动时，观众如果分别去拉主播的流，对带宽成本和用户的设备都有很高的要求。会让服务器混合成一条流，观众只需要拉混合后的流即可。

* 流管理：推拉流鉴权，禁推管理，提供了业务运营必需的多种流管理功能。


【注】即构科技后台架构负责人 祝永坚（jack）技术分享《ZEGO实时音视频服务架构实践》。


# 参考链接

- [ZEGO实时音视频服务架构实践](https://www.toutiao.com/i6889347789877150211/?tt_from=weixin&utm_campaign=client_share&wxshare_count=1&timestamp=1604055480&app=news_article&utm_source=weixin&utm_medium=toutiao_android&use_new_style=1&req_id=20201030185759010131075067450CF486&group_id=6889347789877150211)

