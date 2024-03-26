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
  Serial.println("****************Now we start calibrate pump*****************");
  Serial.println("Drain the water into the measuring container for 10 seconds.");
  Serial.println("Type /1 when you're ready, or type /0 to decline >>>>>>>>>>>");
  waitBuf(30000);
  bool state = false;
  bool tr = true;

  int V = 0;

  state = verCommand("/1", 2);
  
  
  cleanBuf();

  int turns = 3;

  while (state && turns > 0) {
    Serial.println("Type how many millilitres of water you have. ex: $1500 *****");
    Serial.println("which corresponds to 1.5 liters. Use char, $ to start typing");
    waitBuf(30000);
    while (Serial.available() > 0) {
      char first = Serial.read();
      cleanBuf();
      if (first == '$') {
        V = Serial.parseInt();
        state = false;
        cleanBuf();
      } else {
        V = 0;
        Serial.println("****************************ERROR***************************");
        cleanBuf();
        turns--;
      }
    }
  }

  Serial.println("*****************************END****************************");
  Serial.print(V);
  Serial.println(" millilitres");
  return V;
}

bool verCommand(const char *cmd, int len) {
  char *buf = new char[len];
  Serial.readBytes(buf, len);
  cleanBuf();
  int i = 0;
  while(cmd[i] == buf[i] && i < len){
    i++;
  }
  delete [] buf;

  if(i == len){
    return true;
  }else{
    return false;
  }
}

void cleanBuf(){
  while (Serial.available() > 0) char bin = Serial.read();
}

void waitBuf(uint32_t timeout){
  uint32_t tmr = millis();
  while(!Serial.available() && millis() < tmr + timeout);
}