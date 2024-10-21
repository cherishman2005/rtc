# sdp协议中的packetization-mode方式和三种流传输模式

## 1、rtsp 中的sdp协议

sdp名称为会话描述协议，包括sip协议中也是用这个

rtsp协议中可以传输ps流，ts流 ，裸流也称为es流，es流比较常规，熟悉h264 的rtp封包方式，或者h265封包方式，h265封包RTP可以参考ffmpeg源代码

rtsp协议中 如果传输的是h264 h265

“m=” 行中的媒体名为 “video”

“a=rtpmap” 行中的编码名称是 H264 h265

“a=rtpmap” 行中的时钟频率一般是 90000，可以是别的数字，但是90000这个数字对很多帧率都是适应的，所以用它。

其他参数都包括在 “a=fmtp” .

## 2、pm方式

packetization-mode:

表示支持的封包模式.

1 、packetization-mode 值为 0 时或不存在时, 必须使用单一 NALU 单元模式.

2、 packetization-mode 值为 1 时使用非交错(non-interleaved)封包模式.

3、 packetization-mode 值为 2 时使用交错(interleaved)封包模式.

profile-level-id:

这个参数用于指示 H.264 流的 profile 类型和级别. 由 Base16(十六进制) 表示的 3 个字节. 第一个字节表示 H.264 的 Profile 类型, 第三个字节表示 H.264 的 Profile 级别

我们一般使用packetization-mode = 1，非交错模式

## 3、ps 、ts、es传输模式

以下用代码来恢复rtsp的场景传输三种不同的流
```Cpp
int handle_cmd(string& cmd, string& CSeq, uint32_t sessionid, char* cmdres,int ps_n_ts)
	{
		#define BUFSIZE 8*1024

		int h26xtype = 96;
		if (cmd.compare("DESCRIBE") == 0)
		{
			const char * psnts;
			if (ps_n_ts == 1)
			{
				psnts = "a=rtpmap:32 MP2P/90000";
				h26xtype = 32;
			}
			else if (ps_n_ts == 2)
			{
				psnts = "a=rtpmap:96 H264/90000";
				h26xtype = 96;
			}
			else if (ps_n_ts == 3)
			{
				psnts = "a=rtpmap:33 MP2T/90000";
				h26xtype = 33;
			}
			char sdp[4096];
			sprintf(sdp, "v=0\r\n"
				"o=shyjx 22345 22345 IN IP4 %s\r\n"
				"s=H.264 Video, streamed by qb\r\n"
				"t=0 0\r\n"
				//"m=video 0 RTP/AVP 96\r\n"
				"%s\r\n"
				"c=IN IP4 0.0.0.0\r\n"
				"a=rtpmap:96 H264/90000\r\n"
				"a=fmtp:96 packetization-mode=1;\r\n"
				"a=control:stream", m_strlocalip.c_str(), psnts, h26xtype);
			return buildDescribeRes(cmdres, BUFSIZE, sdp, CSeq.c_str());
		}
		else if (cmd.compare("OPTIONS") == 0)
		{
			return buildOptionRes(cmdres, BUFSIZE, CSeq.c_str());
		}
		else if (cmd.compare("SETUP") == 0)
		{
			return buildSetupTcpRes(cmdres, BUFSIZE, 0, 1, sessionid, CSeq.c_str());
		}
		else if (cmd.compare("PLAY") == 0)
		{
			return buildPlayRes(cmdres, BUFSIZE, NULL, sessionid, CSeq.c_str());
		}
		else if (cmd.compare("TEARDOWN") == 0)
		{
			return buildTeardownRes(cmdres, BUFSIZE, sessionid, CSeq.c_str());
		}
		return -1;
	}
```
