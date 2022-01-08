package com.qnydesk.server.handler;

import io.netty.buffer.Unpooled;
import io.netty.channel.ChannelHandler;
import io.netty.channel.ChannelHandlerContext;
import io.netty.channel.ChannelInboundHandlerAdapter;
import io.netty.util.CharsetUtil;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Component;

@Component
@ChannelHandler.Sharable
public class NettyServerHandlerEnd extends ChannelInboundHandlerAdapter {
    private Logger logger = LoggerFactory.getLogger(getClass());

    @Override
    public void handlerAdded(ChannelHandlerContext ctx) throws Exception{
        ctx.channel().writeAndFlush(Unpooled.copiedBuffer("Hello!", CharsetUtil.UTF_8));        // 向客户端发送信息
    }

    @Override
    public  void channelRead(ChannelHandlerContext ctx, Object msg) throws  Exception{
        // 接收到客户端信息
        logger.info("Client: {}", msg.toString());
    }
}
