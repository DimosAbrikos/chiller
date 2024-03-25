#include "types.h"
#include "inputs.h"

heatFlow hFlow();

#include <microDS18B20.h>
MicroDS18B20<SENSOR1_PIN> sensor1;
MicroDS18B20<SENSOR2_PIN> sensor2;

uint32_t tmr;
uint32_t period = 5000; // min 800

#include "functions.h"

void setup(){
    Serial.begin(9600);
    hFlow.setState(pPump);
    sensor1.requestTemp();
    sensor2.requestTemp();
    tmr = millis();
    delay(1000);
}

void loop(){

    if(millis() - tmr > period){
        hFlow.tick(getTemp(0), getTemp(1), millis());
        display(hFlow.getCoolingPower(), hFlow.getHeatingPower());
    }
    
}