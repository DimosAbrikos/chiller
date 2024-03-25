typedef struct{
    int pumpPower;


}settings;

const float WATER_HEAT_C = 4200;

class heatFlow{

    public:

    void setState(float pumpPower){
        pPower = pumpPower;
    }

    void tick(float inputTemp, float outTemp, uint32_t time){
        this->inputTemp = inputTemp;
        this->outTemp = outTemp;
        calc(time);
        lastTime = time;
    }

    float getDeltaHeat(){return deltaHeat;}
    float getCoolPower(){return coolPower;}

    private:

    float inputTemp;
    float outTemp;
    float deltaHeat;
    float pPower;
    float coolPower;

    uint32_t lastTime;

    void calc(uint32_t time){
        float per = (time - lastTime)/1000;
        deltaHeat = WATER_HEAT_C * pPower * per * (outTemp - inputTemp);
        coolPower = deltaHeat * per;
    }

}