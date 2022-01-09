package com.qnydesk.server.handler;

import com.qnydesk.common.entity.QnyHost;
import com.qnydesk.protocol.BigPack;
import io.netty.channel.Channel;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Component;

@Component
public class NettyChannelManager {
    private Logger logger = LoggerFactory.getLogger(getClass());

    //注册主机信息
    public void dealHostInfo(BigPack.CsHostInfo csHostInfo, Channel channel) {
        String deviceId;
        //加入设备
        QnyHost host = new QnyHost();
        if (0 == csHostInfo.getActive()) {
            //被控用CPUID作为设备ID
            deviceId = csHostInfo.getCpuId().substring(0, 9);
            System.out.println(deviceId);
        } else if (1 == csHostInfo.getActive()) {
            //主控用netty自带channel短ID
            deviceId = channel.id().asShortText();

        } else {
            return;
        }

    }
}
