package com.example.demo.controller;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.web.bind.annotation.*;

   @SpringBootApplication
   @RestController
   public class UserProfileController {

       private String arduinoData = "";  // 아두이노로부터 받은 데이터를 저장할 변수

       @GetMapping("/getData")
       public String getData() {
           return arduinoData;  // 클라이언트에게 데이터 제공
       }

       @PostMapping("/updateData")
       public void updateData(@RequestBody String newData) {
           arduinoData = newData;  // 아두이노에서 데이터 업데이트
       }

       public static void main(String[] args) {
           SpringApplication.run(UserProfileController.class, args);
       }
   }