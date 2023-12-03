package com.example.demo.controller;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.scheduling.annotation.Scheduled;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RestController;

@SpringBootApplication
@RestController
public class UserProfileController {

    private String arduinoData = "";  // 아두이노에서 데이터를 저장할 변수

    @GetMapping("/getData")
    public String getData() {
        return arduinoData;  // 클라이언트에게 데이터 제공
    }

    @PostMapping("/updateData")
    public void updateData(@RequestBody String newData) {
        arduinoData = newData;  // 아두이노에서 데이터 업데이트
    }

    @Scheduled(fixedRate = 5000) // 5초마다 이 메서드 실행 (필요에 따라 조정)
    public void fetchDataFromArduino() {
        // 아두이노에서 데이터를 가져와서 arduinoData를 업데이트하는 로직을 여기에 구현
        // 예를 들어, 아두이노와 통신하기 위해 라이브러리나 API를 사용할 수 있다.
    }

    public static void main(String[] args) {
        SpringApplication.run(UserProfileController.class, args);
    }
}
