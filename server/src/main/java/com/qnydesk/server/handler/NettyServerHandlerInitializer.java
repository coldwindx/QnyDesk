package com.qnydesk.server.handler;

import io.netty.channel.Channel;
import io.netty.channel.ChannelInitializer;
import io.netty.channel.ChannelPipeline;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

@Component
public class NettyServerHandlerInitializer extends ChannelInitializer<Channel> {
    @Autowired
    private NettyServerHandlerEnd nettyServerHandlerEnd;

    @Override
    protected void initChannel(Channel channel) throws Exception {
        // <1> 获得Channel对应的ChannelPipeline
        ChannelPipeline channelPipeline = channel.pipeline();
        // <2> 添加NettyServerHandler到ChannelPipeline
        channelPipeline.addLast(nettyServerHandlerEnd);
    }
}
