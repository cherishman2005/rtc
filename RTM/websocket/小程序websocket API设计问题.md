# 小程序websocket API设计问题

## 微信小程序websocket为啥不支持onMessage直接赋值？

不支持赋值监听：
```
this.socketTask.onOpen = this.onopen.bind(this);
this.socketTask.onError = this.onerror.bind(this);
this.socketTask.onClose = this.onclose.bind(this);
this.socketTask.onMessage = this.onmessage.bind(this);
```

需要注册回调函数来监听：
```
this.socketTask.onOpen(() => this.onopen());
this.socketTask.onError(res => this.onerror(res));
this.socketTask.onClose(() => this.onclose());
this.socketTask.onMessage(res => this.onmessage(res));
```



建议：微信的websocket API设计能否与浏览器的websocket api尽量保持一致，特别是用法。


# 讨论

- [讨论链接](https://developers.weixin.qq.com/community/develop/doc/000e24ecf24f684bbb3b42b1b56800?highLine=onMessage%2520%25E8%25B5%258B%25E5%2580%25BC)