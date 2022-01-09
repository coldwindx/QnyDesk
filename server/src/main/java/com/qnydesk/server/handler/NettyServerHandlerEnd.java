package com.qnydesk.server.handler;

import io.netty.channel.ChannelHandler;
import io.netty.channel.ChannelHandlerContext;
import io.netty.channel.ChannelInboundHandlerAdapter;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

@Component
@ChannelHandler.Sharable
public class NettyServerHandlerEnd extends ChannelInboundHandlerAdapter {
    private Logger logger = LoggerFactory.getLogger(getClass());

    @Autowired
    private NettyChannelManager nettyChannelManager;

    @Override
    public void handlerAdded(ChannelHandlerContext ctx) throws Exception{
        logger.info(ctx.channel().remoteAddress() + "网络连接上！");        // 向客户端发送信息
    }

    @Override
    public  void channelRead(ChannelHandlerContext ctx, Object msg) throws  Exception{
        // 接收到客户端信息
        logger.info("Client: {}", msg.toString());
    }
}
