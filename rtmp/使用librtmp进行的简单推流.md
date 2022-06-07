# 使用librtmp进行的简单推流

## 整体流程
1、建立rtmp连接

2、发送Audio Header 和Video Header

3、发送Audio和Video数据包

## 时间戳处理
需要注意的是，无论是发送视频数据包还是音频数据包，时间戳一定要是递增的，否则会出现音频卡顿，视频闪烁一类的异常情况。

下面是packet时间戳的伪代码：

double m_TimeStamp = 0;
void Send_Audio()
{
    if(m_TimeStamp == 0)    m_TimeStamp = GetTickCount64();
    double TimeStamp = GetTickCount64() - m_TimeStamp;
    SendAudioPacket(TimeStamp);
}
void Send_Video()
{
    if(m_TimeStamp == 0)    m_TimeStamp = GetTickCount64();
    double TimeStamp = GetTickCount64() - m_TimeStamp;
    SendVideoPacket(TimeStamp);
}
下面是构造数据包的代码示例：

    if(nPacketType == RTMP_PACKET_TYPE_AUDIO)
	{
		packet.m_headerType = RTMP_PACKET_SIZE_MEDIUM;
		packet.m_packetType = RTMP_PACKET_TYPE_AUDIO;
		packet.m_hasAbsTimestamp = 0;
		packet.m_nChannel = STREAM_CHANNEL_AUDIO;
		packet.m_nTimeStamp = nTimestamp;
		packet.m_nInfoField2 = m_pRtmp->m_stream_id;
		packet.m_nBodySize = size;
	}
	else if(nPacketType == RTMP_PACKET_TYPE_VIDEO)
	{
		packet.m_headerType = RTMP_PACKET_SIZE_LARGE;
		packet.m_packetType = nPacketType;
		packet.m_hasAbsTimestamp = 0;
		packet.m_nChannel = STREAM_CHANNEL_VIDEO;
		packet.m_nTimeStamp = nTimestamp;
		packet.m_nInfoField2 = m_pRtmp->m_stream_id;
		packet.m_nBodySize = size;
	}
下面是Audio Header部分的代码示例及解析
```
int RtmpStream::rtmp_audio_header(RtmpMetadata &pMetaData)
{
	RTMPPacket packet;
	RTMPPacket_Reset(&packet);
	RTMPPacket_Alloc(&packet, 4);

	//前4位 |1010| 代表音频数据编码类型为 AAC
	//接下来 2 位 |11| 表示采样率为 44kHz
	//接下来 1 位 |1| 表示采样点位数 16bit
	//最低 1 位 |1| 表示双声道。
	//(10101111)B = (AF)H
	packet.m_body[0] = 0xAF;
	//0x00表示传输的音频数据为Audio Sequence Header
	//如果是0x01表示发送的数据为音频data数据。
	packet.m_body[1] = 0x00;
	//AudioSpecificType			5bit	AAC-LC=2	00010
	//samplingFrequencyIndex	4bit	48000=3		0011	44100=4		0100
	//channelConfiguration		4bit	单声道=1	0001	双声道=2	0010
	//frameL.engthFlag			1bit	标志位, 用于表明IMDCT窗口长度, 0
	//dependsOnCoreCoder		1bit	标志位, 表明是否依赖于corecoder, 0
	//extensionFlag				1bit	选择了AAC - LC, 这里必须为0
	//(00010 0011 0010 000)B = (1190)H
	/*
	//48000
	packet.m_body[2] = 0x11;
	packet.m_body[3] = 0x90;
	//44100
	packet.m_body[2] = 0x12;
	packet.m_body[3] = 0x10;
	*/
	packet.m_body[2] |= (1 << 4);
    //自适应单通道或双通道
	if (pMetaData.nAudioChannels == 1)
		packet.m_body[3] |= (1 << 3);
	else if (pMetaData.nAudioChannels == 2)
		packet.m_body[3] |= (1 << 4);

    //自适应采样频率32000、44100、48000
	if (pMetaData.nAudioSampleSize == 48000)
	{
		packet.m_body[2] |= (1);
		packet.m_body[3] |= (1 << 7);
	}
	else if (pMetaData.nAudioSampleSize == 44100)
	{
		packet.m_body[2] |= (1 << 1);
	}
	else if (pMetaData.nAudioSampleSize == 32000)
	{
		packet.m_body[2] |= (1 << 1);
		packet.m_body[3] |= (1 << 7);
	}

	packet.m_headerType = RTMP_PACKET_SIZE_LARGE;
	packet.m_packetType = RTMP_PACKET_TYPE_AUDIO;
	packet.m_hasAbsTimestamp = 0;
	packet.m_nChannel = STREAM_CHANNEL_AUDIO;
	packet.m_nTimeStamp = 0;
	packet.m_nInfoField2 = m_pRtmp->m_stream_id;
	packet.m_nBodySize = 4;

	//调用发送接口
	int nRet;
	{
		std::lock_guard<std::mutex> lck(m_mtxRtmp);
		nRet = RTMP_SendPacket(m_pRtmp, &packet, TRUE);
	}
	RTMPPacket_Free(&packet);//释放内存  
	return nRet;
}
```

# 参考链接

- [https://www.adobe.com/content/dam/acom/en/devnet/flv/video_file_format_spec_v10.pdf](https://www.adobe.com/content/dam/acom/en/devnet/flv/video_file_format_spec_v10.pdf)
