#include "types.h"

void heatFlow::setPumpPower(float pumpPower){
    pPower = pumpPower;
}

void heatFlow::tick(float inputTemp, float outTemp, uint32_t time){

    per = (time - lastTime)/1000;

    float dH = calc(this->inputTemp, inputTemp);
    heatPower = dH * per;
    this->inputTemp = inputTemp;
    
    float dC = calc(inputTemp, outTemp);
    coolPower = dC * per;

    lastTime = time;
}

float heatFlow::getCoolingPower(){return coolPower;}
float heatFlow::getHeatingPower(){return heatPower;}

float heatFlow::calc(float t1, float t2){
    return WATER_HEAT_C * pPower * per * (t2 - t1);
}

// ====================================================================================


  eepromSaver::eepromSaver(){
    key = 0x11;
    offset = 0;

    int i = offset;
    if(EEPROM[i] == key){
      readParams();
    }else{
      defParams();
      writeKey();
      writeParams();
    }
  }

  void eepromSaver::readParams(){
    EEPROM.get(offset + 1, prms);
  }

  void eepromSaver::defParams(){
    prms.pumpPower = 1000;
    prms.mode = 1;
    prms.counterK = 450;
  }

  void eepromSaver::writeKey(){
    int i = offset;
    EEPROM[i] = key;
  }
  
  void eepromSaver::writeParams(){
    EEPROM.put(offset + 1, prms);
  }