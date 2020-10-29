# RTM JS-SDK API设计技巧

不管是js-sdk还是后端设计，多考虑面向对象的方法。

一个房间对应一个对象实例，附带的操作围绕这个对象展开。

微信，企业微信的IM即时通讯，每建一个群对应一个实例。—— 多借鉴优秀的经验。

不可能是所有群都共用一个实例。如果共用一个实例，出现脏数据怎么清理？
解散群释放资源不彻底等问题。

## 关键API设计

对于一个用户来说，不可能同一时刻多次login/logout操作。所以对login加上mutex锁，做到串行化操作。

login/logout

### login

login登录

login是最重要的API。在底层链接未建立之前，需要做好缓存login请求；待建链后再发送。

![login登录](/img/login.png)

login rpc是一个异步过程(init, pending, finish 3个阶段)：

如果 一个login还在处理过程中pending;  再调用一次login就不合理，会抛出异常。

```
{"rescode":1008,"msg":"login has been locked"}
```

### logout

logout登录

### join

join加入

join加上mutex锁，做到串行化操作。

一个join操作，或包含多个rpc，并且相互关联。—— 在一个join操作时防止触发另外一个join，导致异常。

通过串行化mutex加锁处理，触发另外一个join，就抛出异常。——防止前一个操作被打扰到。

```
{"rescode":1008,"msg":"join has been locked"}
```

### leave

leave离开

## 小结

*  同一实例的 login/logout join/leave sdk成做好串行化操作。

* 异步才做采用标准化的promise/await。

* 业务调用是有用async/await去控制时序。

# Author

zhangbiwu

API设计参考和借鉴了A公司的RTM优秀成功经验。