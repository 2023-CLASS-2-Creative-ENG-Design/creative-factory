package com.example.demo.Dto;

import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.Setter;
import org.springframework.context.annotation.Configuration;

@Setter
@Getter
@Configuration
@AllArgsConstructor
public class DemoDto {
    private int voltage;

    public DemoDto(){
        voltage = 0;
    }
}
