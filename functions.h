#include <Arduino.h>

void display(float cPow, float hPow);
int calibratePump();

bool verCommand(const char *cmd, int len);

void cleanBuf();
void waitBuf(uint32_t timeout);