#include "types.h"
#include "functions.h"

#include "inputs.h"


heatFlow hFlow();

uint32_t tmr;
uint32_t period = 1000;

void setup(){
    Serial.begin(9600);

    hFlow.setState(pPump);
    tmr = millis();
}

void loop(){

    if(millis() - tmr > period){
        hFlow.tick(getTemp(0), getTemp(1), millis());
        display(hFlow.getCoolingPower(), hFlow.getHeatingPower());
    }
    
}