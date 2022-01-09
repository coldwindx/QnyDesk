package com.qnydesk.server.handler;

import com.qnydesk.common.entity.QnyHost;
import com.qnydesk.protocol.BigPack;
import io.netty.channel.Channel;
import io.netty.channel.ChannelId;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Component;

import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentMap;

@Component
public class NettyChannelManager {
    private Logger logger = LoggerFactory.getLogger(getClass());
    // 通过Channel Id查找设备ID
    private ConcurrentMap<ChannelId, String> deviceMap = new ConcurrentHashMap<>();
    // 通过设备ID查找设备信息
    private ConcurrentMap<String, QnyHost> hostMap = new ConcurrentHashMap<>();
    // 在线人数
    private int count = 0;
    //注册主机信息
    public void dealHostInfo(BigPack.CsHostInfo csHostInfo, Channel channel) {
        String deviceId;
        //加入设备
        QnyHost host = new QnyHost();
        if (0 == csHostInfo.getActive()) {
            //被控用CPUID作为设备ID
            deviceId = csHostInfo.getCpuId().substring(0, 9);
            host.setHostId(deviceId);
        } else if (1 == csHostInfo.getActive()) {
            //主控用netty自带channel短ID
            deviceId = channel.id().asShortText();
            host.setHostId(deviceId);
        } else {
            return;
        }
        host.setDeviceId(csHostInfo.getDeviceId());
        host.setCpuId(csHostInfo.getCpuId());
        host.setMacAddress(csHostInfo.getMacAddress());
        host.setChannel(channel);
        addUser(deviceId, host);
    }

    private void addUser(String deviceId, QnyHost host) {
        boolean success = false;
        if(hostMap.containsKey(deviceId)){
            logger.warn("设备[{}]已经登录！", deviceId);
        }else{
            success = true;
            hostMap.put(deviceId, host);
            deviceMap.put(host.getChannel().id(), deviceId);
            if(host.isActive()){
                logger.info("主控设备[{}]连接登录！", deviceId);
            }else{
                ++count;
                logger.info("被控设备[{}]连接登录！", deviceId);
            }
        }

        BigPack.Exchange.Builder exBuilder = BigPack.Exchange.newBuilder();
        exBuilder.setType(BigPack.Exchange.DataType.TypeReplyRegisterDetails);
        exBuilder.setReplyInfo(BigPack.ScReplyInfo.newBuilder().setSuccess(success).setRegisterId(deviceId));
        host.getChannel().writeAndFlush(exBuilder.build());

        logger.info("[被控设备]在线人数：[{}]", count);
    }

    public void remove(Channel channel) {
        String  hostId = deviceMap.get(channel.id());
        deviceMap.remove(channel.id());
        QnyHost host = hostMap.get(hostId);
        hostMap.remove(hostId);
        if(!host.isActive())
            --count;
        logger.info("[被控设备]在线人数：[{}]", count);
    }
}
