#include "types.h"
#include "inputs.h"
#include <microDS18B20.h>

// objects ======================================
heatFlow hFlow;
eepromSaver eepS;
MicroDS18B20<SENSOR1_PIN> sensor1;
MicroDS18B20<SENSOR2_PIN> sensor2;

// gl vars ======================================
uint32_t tmr;
uint32_t period = 5000;  // min 800
bool enable = true;
uint8_t mode = 0;
int waterCnt = 0;

// gl methods ===================================
#include "functions.h"
float getTemp(int ind);
void counterTick(int * cnt, int pin);


void setup() {
  // ==================================
  eepS.readParams();
  hFlow.setPumpPower(eepS.prms.pumpPower);
  mode = eepS.prms.mode;
  // ==================================
  Serial.begin(9600);
  Serial.setTimeout(500);
  sensor1.requestTemp();
  sensor2.requestTemp();
  tmr = millis();
  delay(1000);
}

void loop() {
  if (mode) {
    // ==============================================================

    counterTick(&waterCnt, COUNTER_PIN);

    if (millis() - tmr > period && enable) {
      tmr = millis();
      hFlow.setPumpPower(waterCnt * eepS.prms.counterK);

      hFlow.tick(getTemp(0), getTemp(1), millis());
      display(hFlow.getCoolingPower(), hFlow.getHeatingPower());
    }

    if (Serial.available() > 0) {
      enable = false;
      delay(100);
      if (verCommand("/cal", 4)) {
        int cl = calibratePump();
        if (cl) {
          hFlow.setPumpPower(cl);
          eepS.prms.pumpPower = cl;
          eepS.writeParams();
        }
      }
      enable = true;
    }
  } else {
    // ==============================================================
    if (millis() - tmr > period && enable) {
      tmr = millis();
      hFlow.tick(getTemp(0), getTemp(1), millis());
      display(hFlow.getCoolingPower(), hFlow.getHeatingPower());
    }

    if (Serial.available() > 0) {
      enable = false;
      delay(100);
      if (verCommand("/cal", 4)) {
        int cl = calibratePump();
        if (cl) {
          hFlow.setPumpPower(cl);
          eepS.prms.pumpPower = cl;
          eepS.writeParams();
        }
      }
      enable = true;
    }
  }
}

float getTemp(int ind) {
  if (ind == 0) {

    float t = 0;
    if (sensor1.readTemp()) t = sensor1.getTemp();
    sensor1.requestTemp();
    return t;

  } else if (ind == 1) {

    float t = 0;
    if (sensor2.readTemp()) t = sensor2.getTemp();
    sensor2.requestTemp();
    return t;
  }
}

void counterTick(int * cnt, int pin){
  if(digitalRead(pin)){
    *cnt++;
    delay(100);
  }
}
