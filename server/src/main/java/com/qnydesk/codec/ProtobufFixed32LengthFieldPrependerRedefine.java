package com.qnydesk.codec;

import io.netty.buffer.ByteBuf;
import io.netty.channel.ChannelHandlerContext;
import io.netty.handler.codec.MessageToByteEncoder;

public class ProtobufFixed32LengthFieldPrependerRedefine extends MessageToByteEncoder<ByteBuf> {
    @Override
    protected void encode(ChannelHandlerContext ctx, ByteBuf in, ByteBuf out) throws Exception {
        int bodyLen = in.readableBytes();
        int headLen = 4;
        out.ensureWritable(headLen + bodyLen);
        byte[] headBody = intToBytes(bodyLen);
        out.writeBytes(headBody);
        out.writeBytes(in, in.readerIndex(), bodyLen);
    }

    private byte[] intToBytes(int value) {
        byte[] b = new byte[4];
        b[3] = (byte)(value & 0xff);
        b[2] = (byte)(value >> 8 & 0xff);
        b[1] = (byte)(value >> 16 & 0xff);
        b[0] = (byte)(value >> 24 & 0xff);
        return b;
    }
}
