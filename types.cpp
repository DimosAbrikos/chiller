#include "types.h"

void heatFlow::setState(float pumpPower){
    pPower = pumpPower;
}

void heatFlow::tick(float inputTemp, float outTemp, uint32_t time){

    per = (time - lastTime)/1000;

    float dH = calc(this->inputTemp, inputTemp);
    heatPower = dh * per;
    this->inputTemp = inputTemp;
    
    float dC = calc(inputTemp, outTemp);
    coolPower = dC * per;

    lastTime = time;
}

//float heatFlow::getDeltaHeat(){return deltaHeat;}
float heatFlow::getCoolingPower(){return coolPower;}
float heatFlow::getHeatingPower(){return heatPower;}
//float heatFlow::getDeltaTemp(){return deltaTemp}

float heatFlow::calc(float t1, float t2){
    return WATER_HEAT_C * pPower * per * (t2 - t1);
}