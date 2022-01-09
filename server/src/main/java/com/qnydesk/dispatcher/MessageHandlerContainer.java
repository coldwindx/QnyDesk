package com.qnydesk.dispatcher;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.InitializingBean;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.ApplicationContext;
import java.util.Collection;
import java.util.HashMap;
import java.util.Map;

public class MessageHandlerContainer implements InitializingBean {

    private Logger logger = LoggerFactory.getLogger(getClass());
    /**
     * 消息类型与 MessageHandler 的映射
     */
    private final Map<String, MessageHandler> handlers = new HashMap<>();

    @Autowired
    private ApplicationContext applicationContext;

    // 获得指定类型对应的MessageHandler
    public MessageHandler getMessageHandler(String type) {
        MessageHandler messageHandler = handlers.get(type);
        if(messageHandler == null)
            throw new IllegalArgumentException(String.format("类型(%s)找不到匹配的MessageHandler处理器", type));
        return messageHandler;
    }
    // 注入为Bean后自动调用这个函数
    @Override
    public void afterPropertiesSet() throws Exception {
        // 获得所有的MessageHandler Bean
        Collection<MessageHandler> list = applicationContext.getBeansOfType(MessageHandler.class).values();
        // 加入Map表
        list.forEach(messageHandler->handlers.put(messageHandler.getType(), messageHandler));

        logger.info("[afterPropertiesSet][消息处理器数量：{}]", handlers.size());
    }
}
