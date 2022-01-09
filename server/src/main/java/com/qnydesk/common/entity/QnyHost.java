package com.qnydesk.common.entity;

import io.netty.channel.Channel;
import lombok.Data;

@Data
public class QnyHost {
    private Channel channel;

    private String hostId;

    private String deviceId;

    private String cpuId;

    private String macAddress;

    private int active;

    public boolean isActive()
    {
        return 1 == active;
    }
}
