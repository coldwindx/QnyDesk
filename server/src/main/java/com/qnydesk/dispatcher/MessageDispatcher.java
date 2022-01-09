package com.qnydesk.dispatcher;

import com.qnydesk.protocol.BigPack;
import io.netty.channel.ChannelHandler;
import io.netty.channel.ChannelHandlerContext;
import io.netty.channel.SimpleChannelInboundHandler;
import org.springframework.beans.factory.annotation.Autowired;

import java.util.concurrent.ExecutorService;

import static java.util.concurrent.Executors.newFixedThreadPool;

@ChannelHandler.Sharable
public class MessageDispatcher extends SimpleChannelInboundHandler<BigPack.Exchange> {
    @Autowired
    private MessageHandlerContainer messageHandlerContainer;

    private final ExecutorService executor = newFixedThreadPool(100);

    @Override
    protected void channelRead0(ChannelHandlerContext ctx, BigPack.Exchange exchange) throws Exception {
        // <1> 获得 type 对应的 MessageHandler 处理器
        MessageHandler messageHandler = messageHandlerContainer.getMessageHandler(BigPack.Exchange.class.toGenericString());
        // <2> 执行逻辑
        executor.submit(() -> {
            // noinspection unchecked
            messageHandler.execute(ctx, exchange);
        });
    }
}
