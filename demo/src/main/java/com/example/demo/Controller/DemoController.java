package com.example.demo.Controller;

import com.example.demo.Dto.DemoDto;
import lombok.AllArgsConstructor;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.*;

@RestController
@AllArgsConstructor
public class DemoController {

    private DemoDto re;
    @PostMapping("/demo")
    public void updateVoltage(@RequestBody DemoDto data){
        re.setVoltage(data.getVoltage());
    }

    @GetMapping("/sunflower")
    public String getIndexHtml(){
        String a = "Voltage is " + Integer.toString(re.getVoltage());
        return a;
    }

}
