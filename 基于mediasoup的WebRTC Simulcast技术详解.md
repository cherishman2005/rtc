# 基于 mediasoup 的 WebRTC Simulcast 技术详解

**核心结论**：Simulcast 是 WebRTC 中同时发送多路不同分辨率 / 码率独立视频流的技术，mediasoup 作为高性能 SFU 提供完整支持，可让发送端上传多层流，SFU 根据接收端带宽与需求智能选择转发，在多人会议中实现带宽自适应与体验优化。

## 一、Simulcast 基础概念

- **定义**：Simulcast（联播 / 多流传输）指发送端对同一视频源进行多次独立编码，生成不同分辨率、码率的多路流（通常低 / 中 / 高三层），同时上传至 SFU；SFU 根据接收端网络条件与显示需求，选择最合适的流转发。
- **核心价值**：解决多人会议中 NAT 穿越与带宽差异问题，避免 Mesh 架构的 N² 流量爆炸，兼顾用户体验与系统可扩展性。
- **典型应用**：
  - 网络好的用户接收 1080p 高清流
  - 网络一般的用户接收 720p 中清流
  - 网络差或仅看缩略图的用户接收 360p/180p 低清流

## 二、mediasoup 中 Simulcast 的实现原理

### 1. 核心组件协作流程

```plaintext
摄像头采集 → 浏览器/客户端编码器（多分辨率编码）→ Transport → Producer（多流注入）→ Router → Consumer（按需选择转发）→ 接收端解码播放

Producer：负责注入多路编码流，通过 encodings 数组定义各层参数，每路流有独立 SSRC 与 RID 标识。
Router：SFU 核心，维护流的路由与分发逻辑，跟踪各 Consumer 的带宽状态。
Consumer：订阅流时，mediasoup 根据带宽估算与首选层设置选择最优流，支持动态切换。
```

### 2. 关键技术特点

- 完全符合 WebRTC 标准：基于 RTP 扩展头与 RTCP 反馈机制，兼容主流浏览器（Chrome/Firefox/Safari）。
- 无服务端编解码开销：仅做智能转发，不参与编码 / 解码，保持高性能与低延迟。
- 灵活的层控制：支持设置每路流的 maxBitrate、scaleResolutionDownBy、rid 等参数，精确控制各层质量。
- 实时带宽适配：通过 RTCP RR/XR 与 TWCC 等机制估算下行带宽，快速切换流层，切换延迟通常 <1 秒。

## 三、mediasoup Simulcast 配置实战

### 1. 客户端配置（发送端）

```javascript
// 视频轨道获取
const videoTrack = await navigator.mediaDevices.getUserMedia({ video: true }).then(stream => stream.getVideoTracks()[0]);

// Simulcast编码配置（三层流）
const encodings = [
  { rid: 'low', maxBitrate: 100000, scaleResolutionDownBy: 4 },  // 低清：1/4分辨率，100kbps
  { rid: 'mid', maxBitrate: 300000, scaleResolutionDownBy: 2 },  // 中清：1/2分辨率，300kbps
  { rid: 'high', maxBitrate: 900000 }                            // 高清：原始分辨率，900kbps
];

// 创建Simulcast Producer
const videoProducer = await sendTransport.produce({
  track: videoTrack,
  encodings: encodings,
  codecOptions: {
    videoGoogleStartBitrate: 1000  // 初始码率
  }
});
```

**关键参数说明**：
- rid：分辨率标识（low/mid/high），用于 SFU 识别与切换。
- scaleResolutionDownBy：分辨率缩放因子，控制输出分辨率。
- maxBitrate：最大码率，防止带宽占用过高。

### 2. 服务端配置（Node.js 示例）

```javascript
// 创建Router
const router = await worker.createRouter({ mediaCodecs });

// 处理Producer创建事件
router.on('producer', producer => {
  console.log(`Producer created: ${producer.id}, simulcast: ${producer.simulcast}`);
});

// Consumer配置（设置首选层）
const consumer = await transport.consume({
  producerId: producerId,
  rtpCapabilities: clientRtpCapabilities,
  paused: true  // 先暂停，设置首选层后再恢复
});

// 设置Consumer首选层（根据用户需求或带宽）
await consumer.setPreferredLayers({ spatialLayer: 2 });  // 2=high, 1=mid, 0=low
await consumer.resume();

// 监听层切换事件
consumer.on('layerschange', (layers) => {
  console.log(`Consumer ${consumer.id} switched to layer:`, layers);
  // 通知客户端当前有效层
});
```

**核心 API**：
- producer.simulcast：布尔值，标识是否为 Simulcast 流。
- consumer.setPreferredLayers()：设置接收端首选空间层与时间层。
- layerschange 事件：监听实际传输的层变化，用于信令同步。

### 3. 浏览器兼容性与编解码器

- 支持 H.264、VP8、VP9 等主流编解码器，AV1 实验性支持。
- 浏览器需支持 RTCRtpSender.setParameters() 接口，现代浏览器均已支持。
- 移动端设备建议控制编码层数（2 层为宜），平衡性能与带宽。

## 四、带宽自适应机制详解

- **发送端行为**：浏览器根据编码配置生成多路独立 RTP 流，每路有独立 SSRC 与 PT 值。通过 RTCP 反馈调整各层码率，避免拥塞。
- **SFU 决策逻辑**：基于 RTCP RR/XR 包计算丢包率与延迟，估算接收端下行带宽。结合 setPreferredLayers() 设置的首选层，选择最高可用层。层切换时无缝衔接，避免卡顿，切换延迟 <1 秒。
- **接收端反馈**：客户端通过信令告知服务端显示需求（如主画面 / 缩略图）。服务端根据反馈动态调整 preferredLayers，实现 "看大图用高清，看小图用低清"。

## 五、Simulcast vs SVC：技术对比

| 对比维度 | Simulcast | SVC（可伸缩视频编码） |
|---------|-----------|---------------------|
| 编码方式 | 多路独立编码 | 单路流包含多层空间 / 时间信息 |
| 上行带宽 | 高（多路流叠加） | 低（单路流） |
| CPU 负载 | 高（多次编码） | 低（单次编码） |
| 切换延迟 | 快（<1 秒） | 慢（依赖 GOP 结构） |
| 兼容性 | 广泛（所有浏览器） | 有限（仅 VP9/AV1，Chrome 最佳） |
| 解码复杂度 | 低 | 高 |
| 抗丢包能力 | 弱 | 强（可丢弃增强层） |

**mediasoup 支持**：同时支持 Simulcast 与 SVC，可根据场景灵活选择。中小规模会议推荐 Simulcast（兼容性好），大规模会议或带宽受限场景推荐 SVC。

## 六、最佳实践与注意事项

### 编码层数选择
- 常规场景：2-3 层（低 / 中 / 高），平衡带宽与体验。
- 移动端：2 层（低 / 高），降低 CPU 与带宽消耗。

### 码率与分辨率配置建议

| 层级 | 分辨率 | 码率范围 | 适用场景 |
|-----|-------|---------|---------|
| 低清 | 360p (640×360) | 100-300kbps | 缩略图、网络差 |
| 中清 | 720p (1280×720) | 300-800kbps | 普通网络、小窗口 |
| 高清 | 1080p (1920×1080) | 800-2000kbps | 网络好、主画面 |

### 性能优化技巧
- 启用硬件加速编码（H.264/VP8），降低 CPU 占用。
- 合理设置 maxBitrate，避免带宽浪费。
- 结合 SFU 的带宽估算，动态调整 preferredLayers。

### 常见问题排查
- 仅低清流生效：检查编码器是否支持多分辨率，或 scaleResolutionDownBy 参数是否合理。
- 切换卡顿：调整码率梯度，避免层间码率差异过大；检查网络稳定性。
- 兼容性问题：降级为 2 层编码，或切换至更兼容的编解码器（如 H.264）。

## 七、总结

mediasoup 的 Simulcast 实现为 WebRTC 多人实时通信提供了高效、灵活、标准兼容的带宽自适应方案。通过发送端多流编码与 SFU 智能转发，可在不同网络条件下为用户提供最佳体验，是构建大规模、高质量视频会议系统的核心技术之一。

如需更深入的配置细节，建议参考 mediasoup 官方文档：
- RTP Parameters and Capabilities
- Communication Between Client and Server
