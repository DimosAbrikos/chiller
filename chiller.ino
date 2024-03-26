#include "types.h"
#include "inputs.h"

heatFlow hFlow;

#include <microDS18B20.h>
MicroDS18B20<SENSOR1_PIN> sensor1;
MicroDS18B20<SENSOR2_PIN> sensor2;

uint32_t tmr;
uint32_t period = 5000; // min 800

#include "functions.h"
float getTemp(int ind);
bool enable = true;

void setup(){
    Serial.begin(9600);
    Serial.setTimeout(500);
    hFlow.setState(1000);
    sensor1.requestTemp();
    sensor2.requestTemp();
    tmr = millis();
    delay(1000);
}

void loop(){

    if(millis() - tmr > period && enable){
        tmr = millis();
        hFlow.tick(getTemp(0), getTemp(1), millis());
        display(hFlow.getCoolingPower(), hFlow.getHeatingPower());
    }
    
    if(Serial.available() > 0){
      enable = false;
      delay(100);
      if(verCommand("/cal", 4)){
        int cl = calibratePump();
        if(cl) hFlow.setState(cl);
      }
      enable = true;
    }
    
}

float getTemp(int ind){
    if(ind == 0){

        float t = 0;
        if (sensor1.readTemp()) t = sensor1.getTemp();
        sensor1.requestTemp();
        return t;

    }else if (ind == 1){

        float t = 0;
        if (sensor2.readTemp()) t = sensor2.getTemp();
        sensor2.requestTemp();
        return t;

    }   

}

