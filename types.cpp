#include "types.h"

void heatFlow::setPumpPower(float pumpPower){
    pPower = pumpPower;
}

void heatFlow::tick(float inputTemp, float outTemp, uint32_t time){

    per = (time - lastTime)/1000;

    heatPower = calc(inputTemp, this->inputTemp) * per ;

    this->inputTemp = inputTemp;
  
    coolPower = calc(inputTemp, outTemp) * per;

    lastTime = time;
}

float heatFlow::getCoolingPower(){return coolPower;}
float heatFlow::getHeatingPower(){return heatPower;}

float heatFlow::calc(float t1, float t2){
    return WATER_HEAT_C * pPower * per * (t1 - t2);
}

// ====================================================================================


  eepromSaver::eepromSaver(){
    key = 0x15;
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
    prms.pumpPower = 0.04;
    prms.mode = 1;
    prms.fanMode = 0;
    prms.fanPower = 128;
    prms.counterK = 450;
  }

  void eepromSaver::writeKey(){
    int i = offset;
    EEPROM[i] = key;
  }
  
  void eepromSaver::writeParams(){
    EEPROM.put(offset + 1, prms);
  }