typedef struct{
    int pumpPower;


}settings;

const float WATER_HEAT_C = 4200;

class heatFlow{

    public:

    void setState(float pumpPower);

    void tick(float inputTemp, float outTemp, uint32_t time);

    //float getDeltaHeat();
    float getCoolingPower();
    float getHeatingPower();
    //float getDeltaTemp();

    private:

    float inputTemp;

    float deltaHeat;
    float deltaTemp;
    float pPower;
    float coolPower;
    float heatPower;
    float per;

    uint32_t lastTime;

    void calc(uint32_t time);

}