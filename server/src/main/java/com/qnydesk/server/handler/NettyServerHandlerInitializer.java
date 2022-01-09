package com.qnydesk.server.handler;

import com.qnydesk.codec.ProtobufFixed32FrameDecoderRedefine;
import com.qnydesk.dispatcher.MessageDispatcher;
import com.qnydesk.protocol.BigPack;
import io.netty.channel.Channel;
import io.netty.channel.ChannelInitializer;
import io.netty.channel.ChannelPipeline;
import io.netty.handler.codec.protobuf.ProtobufDecoder;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

@Component
public class NettyServerHandlerInitializer extends ChannelInitializer<Channel> {
    @Autowired
    private NettyServerHandlerEnd nettyServerHandlerEnd;
    @Autowired
    private MessageDispatcher messageDispatcher;

    @Override
    protected void initChannel(Channel channel) throws Exception {
        // <1> 获得Channel对应的ChannelPipeline
        ChannelPipeline channelPipeline = channel.pipeline();
        // <2> 添加NettyServerHandler到ChannelPipeline
        channelPipeline
                // 入站，处理粘包拆包
                .addLast(new ProtobufFixed32FrameDecoderRedefine())
                .addLast("ExchangeProtobufDecoder", new ProtobufDecoder(BigPack.Exchange.getDefaultInstance()))
                .addLast(messageDispatcher)     // 消息分发器
                .addLast(nettyServerHandlerEnd);
    }
}
