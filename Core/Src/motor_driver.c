#include <motot_driver.h>

MotorCharacteristics motorCharacter;
uint16_t ARRView;

void Motor_Init(float rpmMax, float freqMax, float rpsMax)
{
    motorCharacter.FCPU = 180000000.0;
    motorCharacter.freqMax = freqMax;
    motorCharacter.revPSMax = rpsMax;
    motorCharacter.revPMMax = rpmMax;
    motorCharacter.radPMMax = rpmMax * 2 * M_PI;
    motorCharacter.radPSMax = rpsMax * 2 * M_PI;
}

float Speed_Transformations(float value, char *mode)
{
    if (mode == "RevPM")
        motorCharacter.divisionFactor = (motorCharacter.FCPU * motorCharacter.revPMMax) / motorCharacter.freqMax;
    if (mode == "RevPS")
        motorCharacter.divisionFactor = (motorCharacter.FCPU * motorCharacter.revPSMax) / motorCharacter.freqMax;
    if (mode == "RadPM")
        motorCharacter.divisionFactor = (motorCharacter.FCPU * motorCharacter.radPMMax) / motorCharacter.freqMax;
    if (mode == "RadPS")
        motorCharacter.divisionFactor = (motorCharacter.FCPU * motorCharacter.radPSMax) / motorCharacter.freqMax;
    
    return motorCharacter.divisionFactor / value;
}

void Smooth_Change_Speed(float speed)
{
    static SCS_Parameters param;
    param.step = 60.0;
    param.setSpeed = param.previousValue;
    param.divStep = abs(speed - param.previousValue) / param.step;
    if (speed > param.previousValue + param.step)
    {
        for (uint8_t i = 0; i < param.divStep; i++)
        {
            delay(10);
            param.setSpeed += param.step;
            TIM3->ARR = Speed_Transformations(param.setSpeed, "RadPS");
            ARRView = TIM3->ARR;
        }
    }
    else if (speed < param.previousValue - param.step)
    {
        for (uint8_t i = 0; i < param.divStep; i++)
        {
            delay(10);
            param.setSpeed -= param.step;
            TIM3->ARR = Speed_Transformations(param.setSpeed, "RadPS");
            ARRView = TIM3->ARR;
        }
    }
    else
    {
        TIM3->ARR = Speed_Transformations(speed, "RadPS");
        ARRView = TIM3->ARR;
    }
    param.setSpeed = 0;
    param.previousValue = speed;
    delay(5);
}