package com.qnydesk.dispatcher;

import io.netty.channel.ChannelHandlerContext;

public interface MessageHandler<T> {
    // 消息处理逻辑
    void execute(ChannelHandlerContext ctx, T msg);
    // 获取消息类型
    String getType();
}
