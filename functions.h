#include <Arduino.h>

void display(float cPow, float hPow);
int calibratePump();

bool verCommand(const char *cmd, int len);
int verCommand(const char *cmd, int len, int count);

void cleanBuf();
bool waitBuf(uint32_t timeout);