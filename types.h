#include <Arduino.h>
#include <EEPROM.h>

typedef struct {
  int fanPower;
  uint8_t fanMode;
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

  //Устанавливает производительность помпы в мл/сек
  void setPumpPower(float pumpPower);

  //Рассчет
  void tick(float inputTemp, float outTemp, uint32_t time);

  //получить рассеиваемую мощность
  float getCoolingPower();

  //получить мощность нагревателя
  float getHeatingPower();
  // ========================================================================

private:
  // ========================================================================
  // Variables
  // ========================================================================
  float inputTemp;

  // л/сек
  float pPower;

  //Дж
  float coolPower;

  //Дж
  float heatPower;

  //сек
  float per;

  uint32_t lastTime;
  // ========================================================================
  // Methods
  // ========================================================================

  // Считает теплоту t1 - входная температура, t2 - выходная
  float calc(float t1, float t2);
};