#include "types.h"
#include "functions.h"

#include "inputs.h"


heatFlow hFlow();

void setup(){
    Serial.begin(9600);

    hFlow.setState(pPump);
}

void loop(){

    hFlow.tick(getTemp(0), getTemp(1), millis());
}