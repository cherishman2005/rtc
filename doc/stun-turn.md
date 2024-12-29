# stun-turn

## STUN

## TURN

RTCPeerConnection尝试通过UDP建立对等端之间的直接通信。如果失败的话，RTCPeerConnection就会使用TCP进行连接。如果使用TCP还失败的话，可以用TURN服务器作为后备，在终端之间转发数据。

`TURN用于中继对等端之间的音频/视频/数据流，而不是信令数据。`

