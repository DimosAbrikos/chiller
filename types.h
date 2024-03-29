#include <Arduino.h>
#include <EEPROM.h>

typedef struct {
  float pumpPower;
  uint8_t mode;
  float counterK;
}params;

class eepromSaver {
public:  // ===============================================
  eepromSaver();
  params prms;

  void readParams();
  void writeParams();
  void defParams();

private:  // ==============================================
  uint8_t key;
  int offset;

  
  void writeKey();


};

const float WATER_HEAT_C = 4200;  // Теплоемкость воды

class heatFlow {

public:
  // ========================================================================
  // Methods
  // ========================================================================
  void setPumpPower(float pumpPower);
  void tick(float inputTemp, float outTemp, uint32_t time);
  float getCoolingPower();
  float getHeatingPower();
  // ========================================================================

private:
  // ========================================================================
  // Variables
  // ========================================================================
  float inputTemp;
  float pPower;
  float coolPower;
  float heatPower;
  float per;
  uint32_t lastTime;
  // ========================================================================
  // Methods
  // ========================================================================
  float calc(float t1, float t2);
};