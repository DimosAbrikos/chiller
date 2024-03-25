#include "functions.h"

void display(float cPow, float hPow){
    Serial.print("Cooling power: ");
    Serial.print(cPow);
    Serial.print(" Heatimg power: ");
    Serial.println(hPow);
}

int calibratePump(){
    Serial.println("****************Now we start calibrate pump*****************");
    Serial.println("Drain the water into the measuring container for 10 seconds.");
    Serial.println("Type /1 when you're ready, or type /0 to decline >>>>>>>>>>>");

    bool state = false;
    bool tr = true;

    int V = 0;

    while(Serial.available() > 0){
        if(tr){
            char first = Serial.read();
            if(first == '/'){
                char second = Serial.read();
                if(second == '1') {
                    state = true;
                    tr = false;
                }
                if(second == '0') {
                    state = false;
                    tr = false;
                }
            }
        }
        char bin = Serial.read();
    }

    int turns = 3;

    while(state && turns > 0){
        Serial.println("Type how many millilitres of water you have. ex: $1500 *****");
        Serial.println("which corresponds to 1.5 liters. Use char, $ to start typing");
        while(Serial.available() > 0){
            char first = Serial.read();
            if(first == '$'){
                V = Serial.parseInt();
                state = false;  
            }else{
                V = 0;
                Serial.println("****************************ERROR***************************");
                turns--;
            }
        }
    }

    Serial.println("*****************************END****************************");
    Serial.print(V);
    Serial.println(" millilitres");
    return V;
}