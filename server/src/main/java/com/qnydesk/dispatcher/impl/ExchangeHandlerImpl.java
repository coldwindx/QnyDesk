package com.qnydesk.dispatcher.impl;

import com.qnydesk.dispatcher.MessageHandler;
import com.qnydesk.protocol.BigPack;
import com.qnydesk.server.handler.NettyChannelManager;
import io.netty.channel.ChannelHandlerContext;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;
import org.springframework.util.ObjectUtils;

@Component
public class ExchangeHandlerImpl implements MessageHandler<BigPack.Exchange> {

    private Logger logger = LoggerFactory.getLogger(getClass());

    @Autowired
    NettyChannelManager nettyChannelManager;

    @Override
    public void execute(ChannelHandlerContext ctx, BigPack.Exchange msg) {
        //根据dataType 来显示不同的信息
        BigPack.Exchange.DataType dataType = msg.getType();
        //注册设备
        if (dataType == BigPack.Exchange.DataType.TypeRegisterHost) {
            BigPack.CsHostInfo csHostInfo = msg.getHostInfo();
            nettyChannelManager.dealHostInfo(csHostInfo,ctx.channel());
            return ;
        }
        //源ID
        String resourceId = msg.getResourceId();
        //目标ID
        String targetId = msg.getTargetId();
        if (ObjectUtils.isEmpty(dataType) || ObjectUtils.isEmpty(resourceId)) {
            logger.error("协议类型为空 或者 resourceId没有携带,不处理！");
            return;
        }

    }

    @Override
    public String getType() {
        return BigPack.Exchange.class.toGenericString();
    }

}