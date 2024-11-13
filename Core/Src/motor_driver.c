#include "motot_driver.h"

void Motor_Init(float rpmMax, float freqMax, float rpsMax)
{
    motorCharacter->FCPU = 180000000.0;
    motorCharacter->freqMax = freqMax;
    motorCharacter->revPSMax = rpsMax;
    motorCharacter->revPMMax = rpmMax;
    motorCharacter->radPMMax = rpmMax * 2 * M_PI;
    motorCharacter->radPSMax = rpsMax * 2 * M_PI;
}

float Speed_Transformations(float value, SCS_Mode mode)
{
    if (mode == REV_PER_MIN)
        motorCharacter->divisionFactor = (motorCharacter->FCPU * motorCharacter->revPMMax) / motorCharacter->freqMax;
    if (mode == REV_PER_SEC)
        motorCharacter->divisionFactor = (motorCharacter->FCPU * motorCharacter->revPSMax) / motorCharacter->freqMax;
    if (mode == RAD_PER_MIN)
        motorCharacter->divisionFactor = (motorCharacter->FCPU * motorCharacter->radPMMax) / motorCharacter->freqMax;
    if (mode == RAD_PER_SEC)
        motorCharacter->divisionFactor = (motorCharacter->FCPU * motorCharacter->radPSMax) / motorCharacter->freqMax;
    
    return motorCharacter->divisionFactor / value;
}

float angleCalculate;

void Angle_Transformations(float value){
    angleCalculate = value * 10000.0;
}

SCS_Parameters param;
void Smooth_Change_Speed(float speed, SCS_Mode mode)
{
    // static SCS_Parameters param;
    param.step = 60.0;
    param.setSpeed = param.previousValue;
    param.divStep = abs(speed - param.previousValue) / param.step;
    if (speed > param.previousValue + param.step)
    {
        for (uint8_t i = 0; i < param.divStep; i++)
        {
            delay(60);
            param.setSpeed += param.step;
            TIM3->ARR = Speed_Transformations(param.setSpeed, mode);
            // ARRView = TIM3->ARR;
        }
    }
    else if (speed < param.previousValue - param.step)
    {
        for (uint8_t i = 0; i < param.divStep; i++)
        {
            delay(60);
            param.setSpeed -= param.step;
            TIM3->ARR = Speed_Transformations(param.setSpeed, mode);
            // ARRView = TIM3->ARR;
        }
    }
    else if (speed == 0){
        TIM3->ARR = 0;
    }
    else
    {
        TIM3->ARR = Speed_Transformations(speed, mode);
        // ARRView = TIM3->ARR;
    }
    param.setSpeed = 0;
    param.previousValue = speed;
    delay(5);
}