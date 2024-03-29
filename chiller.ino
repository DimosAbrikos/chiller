#include "types.h"
#include "inputs.h"
#include "consts.h"
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
bool prt = true;
uint8_t mode = 1;
int waterCnt = 0;

// gl methods ===================================
float getTemp(int ind);
bool counterTick(int* cnt, int pin);
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
      float pow = (waterCnt / eepS.prms.counterK) / (period / 1000);
      hFlow.setPumpPower(pow);

      hFlow.tick(getTemp(0), getTemp(1), millis());
      if (prt) display(hFlow.getCoolingPower(), hFlow.getHeatingPower());
      waterCnt = 0;
    }
    everyTick();


  } else {
    // ==============================================================
    if (millis() - tmr > period && enable) {
      tmr = millis();
      hFlow.tick(getTemp(0), getTemp(1), millis());
      if (prt) display(hFlow.getCoolingPower(), hFlow.getHeatingPower());
    }
    everyTick();
  }
}

void everyTick() {
  if (Serial.available() > 0) {
    enable = false;
    delay(100);
    int c = term.verCommand("/get/set", 4, 2);
    switch (c) {
      case 0:
        term.cleanBuf();
        display(hFlow.getCoolingPower(), hFlow.getHeatingPower());
        break;
      case 1:
        term.cleanBuf();
        settings();
        break;
      default:
        term.cleanBuf();
        break;
    }
    enable = true;
  }
}

void settings() {
  Serial.println(strings[0]);
  Serial.println(strings[1]);
  Serial.println(strings[2]);
  Serial.println(strings[3]);
  Serial.println(strings[4]);
  Serial.println(strings[5]);
  Serial.println(strings[6]);
  Serial.println(strings[0]);
  term.cleanBuf();
  term.waitBuf(60000);
  int cmd = term.verCommand("/cal/lst/rep/chm/apr", 4, 5);
  switch (cmd) {
    case 0:
      term.cleanBuf();
      if (!mode) {
        int cl = calibratePump();
        if (cl) {
          hFlow.setPumpPower(cl);
          eepS.prms.pumpPower = cl;
          eepS.writeParams();
        }
      } else {
        int cl = calibrateWaterCounter();
        if (cl) {
          eepS.prms.counterK = cl;
          eepS.writeParams();
        }
      }
      break;

    case 1:
      term.cleanBuf();
      Serial.println(strings[0]);
      Serial.print("# Mode: ");
      Serial.print(mode);
      Serial.print(" counter K: ");
      Serial.print(eepS.prms.counterK);
      Serial.print(" pump power: ");
      Serial.println(eepS.prms.pumpPower);
      Serial.println(strings[0]);

      break;

    case 2:
      term.cleanBuf();
      Serial.println(strings[0]);
      Serial.println(strings[7]);
      Serial.println(strings[0]);
      eepS.defParams();
      eepS.writeParams();
      mode = eepS.prms.mode;
      break;

    case 3:
      term.cleanBuf();
      Serial.println(strings[0]);
      Serial.println("#                    Changing work mode ...                    #");
      Serial.print("# Previos mode: ");
      Serial.println(mode);

      eepS.prms.mode = !eepS.prms.mode;
      mode = eepS.prms.mode;
      Serial.print("# Active mode: ");
      Serial.println(mode);
      Serial.println(strings[0]);
      eepS.writeParams();
      break;

    case 4:
      term.cleanBuf();
      Serial.println(strings[0]);
      Serial.println("# Changing print mode ...");
      Serial.print("# Previos mode: ");
      if (prt) {
        Serial.println("auto");
      } else {
        Serial.println("querry");
      }

      prt = !prt;

      Serial.print("# Active mode: ");
      if (prt) {
        Serial.println("auto");
      } else {
        Serial.println("querry");
      }
      Serial.println(strings[0]);

      break;

    default:
      term.cleanBuf();
      Serial.println(strings[0]);
      Serial.println(strings[8]);
      Serial.println(strings[0]);
      break;
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

bool counterTick(int* cnt, int pin) {
  static bool fl = false;
  bool fl1 = false;
  if (digitalRead(pin) && fl == false) {
    fl = true;
  }

  if (digitalRead(pin) == 0 && fl == true) {
    fl = false;
    (*cnt)++;
    fl1 = true;
  }
  return fl1;
}



void display(float cPow, float hPow) {
  Serial.flush();
  Serial.print("Cooling power: ");
  Serial.print(cPow);
  Serial.print(" Heatimg power: ");
  Serial.println(hPow);
  Serial.flush();
}

int calibratePump() {
  term.cleanBuf();
  Serial.println(strings[0]);
  Serial.println("# Now we start calibrate pump");
  Serial.println("# Type /y when you're ready, or type /n to decline");
  Serial.println(strings[0]);
  Serial.flush();
  term.cleanBuf();
  if (!term.waitBuf(60000)) return 0;
  bool state = false;
  bool tr = true;
  int V = 0;
  state = term.verCommand("/y", 2);

  int turns = 3;

  while (state && turns > 0) {
    Serial.println(strings[0]);
    Serial.println("# Type how many millilitres of water you have. ex: >1500 ");
    Serial.println("# which corresponds to 1.5 liters. Use char, > to start typing");
    Serial.println(strings[0]);
    term.cleanBuf();
    if (!term.waitBuf(60000)) return 0;
    while (Serial.available() > 0) {
      char first = Serial.read();
      term.cleanBuf();
      if (first == '>') {
        V = Serial.parseInt();
        state = false;
        term.cleanBuf();
      } else {
        V = 0;
        Serial.println(strings[0]);
        Serial.println("# ERROR");
        Serial.println(strings[0]);
        term.cleanBuf();
        turns--;
      }
    }
  }

  Serial.println(strings[0]);
  Serial.println("# COMPLITE");
  Serial.println(strings[0]);
  Serial.print("# ");
  Serial.print(V);
  Serial.println(" millilitres");
  Serial.println(strings[0]);
  return V;
}

int calibrateWaterCounter() {
  term.cleanBuf();
  Serial.println(strings[0]);
  Serial.println("# Now we start calibrate water counter");
  Serial.println("# Drain 1 liter of water into the measuring container");
  Serial.println("# Type /y when you're ready, after type /n to end");
  Serial.println(strings[0]);
  Serial.flush();
  term.cleanBuf();
  if (!term.waitBuf(60000)) return 0;
  bool state = false;
  bool tr = true;
  int V = 0;
  state = term.verCommand("/y", 2);



  int turns = 3;

  if (state) {
    Serial.println(strings[0]);
    Serial.println("# Counting ...");
    Serial.println(strings[0]);
    bool mes = true;
    while (mes) {
      if (counterTick(&V, COUNTER_PIN)) Serial.println(V);
      if (Serial.available()) {
        if (term.verCommand("/n", 2)) mes = false;
      }
      delay(5);
    }

  } else {
    Serial.println(strings[0]);
    Serial.println("# EXIT WITHOUT SAVING");
    Serial.println(strings[0]);
    return 0;
  }

  Serial.println(strings[0]);
  Serial.println("# COMPLITE");
  Serial.println(strings[0]);
  Serial.print("# ");
  Serial.print(V);
  Serial.println(" Pulses");
  Serial.println(strings[0]);
  return V;
}
