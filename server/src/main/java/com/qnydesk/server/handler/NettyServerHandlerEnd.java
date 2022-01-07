package com.qnydesk.server.handler;

import io.netty.channel.ChannelHandler;
import io.netty.channel.ChannelHandlerContext;
import io.netty.channel.ChannelInboundHandlerAdapter;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Component;

@Component
@ChannelHandler.Sharable
public class NettyServerHandlerEnd extends ChannelInboundHandlerAdapter {
    private Logger logger = LoggerFactory.getLogger(getClass());

    @Override
    public void handlerAdded(ChannelHandlerContext ctx) throws Exception{
        logger.info(ctx.channel().remoteAddress() + "网络连接上" + ctx.channel().id().asLongText());
    }
}
