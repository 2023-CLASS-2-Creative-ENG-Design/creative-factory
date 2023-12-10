package com.example.demo.controller;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.scheduling.annotation.EnableScheduling;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RestController;

import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.HashMap;
import java.util.Map;

@SpringBootApplication
@EnableScheduling // 스케줄링 활성화
@RestController
public class UserProfileController {

    private Map<String, Double> arduinoData = new HashMap<>();  // 아두이노에서 데이터를 저장할 변수

    @GetMapping("/getData")
    public Map<String, Double> getData() {
        return arduinoData;  // 클라이언트에게 데이터 제공
    }

    @PostMapping("/updateData")
    public void updateData(@RequestBody Map<String, Double> data) {
        // 받아온 데이터를 현재 시간과 함께 저장
        LocalDateTime currentTime = LocalDateTime.now();
        DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss");
        String formattedTime = currentTime.format(formatter);

        arduinoData.put(formattedTime, data.get("value"));
    }

    public static void main(String[] args) {
        SpringApplication.run(UserProfileController.class, args);
    }
}


