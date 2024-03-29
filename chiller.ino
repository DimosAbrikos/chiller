#include "types.h"
#include "inputs.h"
#include <microDS18B20.h>
#include <SerialTerminal.h>

// objects ======================================

heatFlow hFlow;
eepromSaver eepS;
MicroDS18B20<SENSOR1_PIN> sensor1;
MicroDS18B20<SENSOR2_PIN> sensor2;
terminal term;

// gl vars ======================================
uint32_t tmr;
uint32_t period = 5000;  // min 800
bool enable = true;
uint8_t mode = 0;
int waterCnt = 0;

// gl methods ===================================
float getTemp(int ind);
void counterTick(int * cnt, int pin);
void display(float cPow, float hPow);
int calibratePump();

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
      if (term.verCommand("/cal", 4)) {
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
      if (term.verCommand("/cal", 4)) {
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


void display(float cPow, float hPow) {
  Serial.print("Cooling power: ");
  Serial.print(cPow);
  Serial.print(" Heatimg power: ");
  Serial.println(hPow);
  Serial.flush();
}

int calibratePump() {
  term.cleanBuf();
  Serial.println("# ============================================================ #");
  Serial.println("#                 Now we start calibrate pump                  #");
  Serial.println("# Drain the water into the measuring container for 10 seconds. #");
  Serial.println("# Type /y when you're ready, or type /n to decline             #");
  Serial.println("# ============================================================ #");
  if(!term.waitBuf(60000))return 0;
  bool state = false;
  bool tr = true;
  int V = 0;
  state = term.verCommand("/y", 2);

  int turns = 3;

  while (state && turns > 0) {
    Serial.println("# ============================================================ #");
    Serial.println("# Type how many millilitres of water you have. ex: >1500       #");
    Serial.println("# which corresponds to 1.5 liters. Use char, > to start typing #");
    Serial.println("# ============================================================ #");
    term.cleanBuf();
    if(!term.waitBuf(60000))return 0;
    while (Serial.available() > 0) {
      char first = Serial.read();
      term.cleanBuf();
      if (first == '>') {
        V = Serial.parseInt();
        state = false;
        term.cleanBuf();
      } else {
        V = 0;
        Serial.println("# ============================================================ #");
        Serial.println("#                             ERROR                            #");
        Serial.println("# ============================================================ #");
        term.cleanBuf();
        turns--;
      }
    }
  }

  Serial.println("# ============================================================ #");
  Serial.println("#                           COMPLITE                           #");
  Serial.println("# ============================================================ #");
  Serial.print("# ");
  Serial.print(V);
  Serial.println(" millilitres");
  Serial.println("# ============================================================ #");
  return V;
}
