#include <Arduino.h>

typedef struct{
    int pumpPower;


}settings;

const float WATER_HEAT_C = 4200; // Теплоемкость воды

class heatFlow{

    public:
    // ========================================================================
    // Methods
    // ========================================================================
    void setState(float pumpPower);
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