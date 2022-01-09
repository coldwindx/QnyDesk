package com.qnydesk;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;

@SpringBootApplication
public class ServerApplication {

    public static void main(String[] args) {
        try{
            SpringApplication.run(ServerApplication.class, args);
        }catch (Exception e){
            e.printStackTrace();
        }
    }

}
