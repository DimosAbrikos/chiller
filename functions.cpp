#include "functions.h"

void display(float cPow, float hPow) {
  Serial.print("Cooling power: ");
  Serial.print(cPow);
  Serial.print(" Heatimg power: ");
  Serial.println(hPow);
  Serial.flush();
}

int calibratePump() {
  cleanBuf();
  Serial.println("# ============================================================ #");
  Serial.println("#                 Now we start calibrate pump                  #");
  Serial.println("# Drain the water into the measuring container for 10 seconds. #");
  Serial.println("# Type /y when you're ready, or type /n to decline             #");
  Serial.println("# ============================================================ #");
  if(!waitBuf(60000))return 0;
  bool state = false;
  bool tr = true;
  int V = 0;
  state = verCommand("/y", 2);

  int turns = 3;

  while (state && turns > 0) {
    Serial.println("# ============================================================ #");
    Serial.println("# Type how many millilitres of water you have. ex: >1500       #");
    Serial.println("# which corresponds to 1.5 liters. Use char, > to start typing #");
    Serial.println("# ============================================================ #");
    cleanBuf();
    if(!waitBuf(60000))return 0;
    while (Serial.available() > 0) {
      char first = Serial.read();
      cleanBuf();
      if (first == '>') {
        V = Serial.parseInt();
        state = false;
        cleanBuf();
      } else {
        V = 0;
        Serial.println("# ============================================================ #");
        Serial.println("#                             ERROR                            #");
        Serial.println("# ============================================================ #");
        cleanBuf();
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

bool verCommand(const char *cmd, int len) {
  char *buf = new char[len];
  Serial.readBytes(buf, len);
  cleanBuf();
  int i = 0;
  while (cmd[i] == buf[i] && i < len) {
    i++;
  }
  delete[] buf;

  if (i == len) {
    return true;
  } else {
    return false;
  }
}

int verCommand(const char *cmd, int len, int count){
  int blen = len * count;
  char *buf = new char[len];
  Serial.readBytes(buf, len);
  cleanBuf();
  
  int j = 0;
  for(j = 0; j < count; j++){
    int i = 0; 
    int ic = j * len;
    while (cmd[ic] == buf[i] && i < len) {
      i++;
      ic++;
      if (i == len) {
        delete[] buf;
        return j;
      } 
    }
  }
  return -1;

}

void cleanBuf() {
  while (Serial.available() > 0) char bin = Serial.read();
}

bool waitBuf(uint32_t timeout) {
  uint32_t tmr = millis();
  bool res = true;
  while (!Serial.available() && res){
    if(millis() > tmr + timeout) res = false; 
  }
  return res;
}