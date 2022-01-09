package com.qnydesk.codec;

import io.netty.buffer.ByteBuf;
import io.netty.channel.ChannelHandlerContext;
import io.netty.handler.codec.ByteToMessageDecoder;
import io.netty.handler.codec.CorruptedFrameException;

import java.util.List;

public class ProtobufFixed32FrameDecoderRedefine extends ByteToMessageDecoder {
    public  ProtobufFixed32FrameDecoderRedefine() {

    }
    /**
     * @Param ByteBuf in: 来自客户端的消息
     * @Param List<Object> out: 处理后的输出
     */
    @Override
    protected void decode(ChannelHandlerContext ctx, ByteBuf in, List<Object> out) throws Exception {
        in.markReaderIndex();
        int preIndex = in.readerIndex();
        in.markReaderIndex();
        // <1> 校验
        if(in.readableBytes() < 4)
            throw new CorruptedFrameException("Message less min length[4]:" + in.readableBytes());
        // <2> 读取消息长度
        byte[] lenthBytes = new byte[4];
        in.readBytes(lenthBytes);
        int length = bytesToInt(lenthBytes);
        // <3> 尝试读取消息
        if(preIndex != in.readerIndex()){
            if(length < 0)
                throw new CorruptedFrameException("Message length is negative:" + length);
            if(in.readableBytes() < length){
                in.resetReaderIndex(); // 发生拆包
            }else{
                out.add(in.readRetainedSlice(length));  // 读相应长度数据
            }
        }
    }

    private int bytesToInt(byte[] b) {
        return b[3] & 0xff | (b[2] & 0xff) << 8 | (b[1] & 0xff) << 16 | (b[0] & 0xff) << 24;
    }
}
