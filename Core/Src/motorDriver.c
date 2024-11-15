#include "../Inc/motorDriver.h"
#include "../Inc/interrupt.h"

Motor_Characteristics motorCharacter;
Angular_Parameters angular;

void Motor_Init(float rpmMax, float freqMax, float rpsMax, Calc_Mode mode)
{
    // motorCharacter->FCPU = 180000000.0;
    // motorCharacter->PPS = pulsePerSecond;
    // motorCharacter->freqMax = freqMax;
    // motorCharacter->revPSMax = rpsMax;
    // motorCharacter->revPMMax = rpmMax;
    // motorCharacter->radPMMax = rpmMax * 2 * M_PI;
    // motorCharacter->radPSMax = rpsMax * 2 * M_PI;
    motorCharacter.FCPU = 180000000.0;
    motorCharacter.oneRPS = 10000.0;
    motorCharacter.freqMax = freqMax;
    motorCharacter.revPSMax = rpsMax;
    motorCharacter.revPMMax = rpmMax;
    motorCharacter.radPMMax = rpmMax * 2 * M_PI;
    motorCharacter.radPSMax = rpsMax * 2 * M_PI;
    switch (mode)
    {
    case REV_PER_MIN:
        motorCharacter.divisionFactor = (motorCharacter.FCPU * motorCharacter.revPMMax) / motorCharacter.freqMax;
        // motorCharacter->divisionFactor = (motorCharacter->FCPU * motorCharacter->revPMMax) / motorCharacter->freqMax;
        break;
    case REV_PER_SEC:
        motorCharacter.divisionFactor = (motorCharacter.FCPU * motorCharacter.revPSMax) / motorCharacter.freqMax;
        // motorCharacter->divisionFactor = (motorCharacter->FCPU * motorCharacter->revPSMax) / motorCharacter->freqMax;
        break;
    case RAD_PER_MIN:
        motorCharacter.divisionFactor = (motorCharacter.FCPU * motorCharacter.radPMMax) / motorCharacter.freqMax;
        // motorCharacter->divisionFactor = (motorCharacter->FCPU * motorCharacter->radPMMax) / motorCharacter->freqMax;
        break;
    case RAD_PER_SEC:
        motorCharacter.divisionFactor = (motorCharacter.FCPU * motorCharacter.radPSMax) / motorCharacter.freqMax;
        // motorCharacter->divisionFactor = (motorCharacter->FCPU * motorCharacter->radPSMax) / motorCharacter->freqMax;
        break;
    }
}

float Speed_Transformation_In_ARR(float value)
{
    return motorCharacter.divisionFactor / value;
}

void Set_Number_Of_Revolutions(float taskDegree, float taskAngularVelocity, TIM_TypeDef *arrTIM)
{
    motorCharacter.freq = motorCharacter.FCPU / Speed_Transformation_In_ARR(taskAngularVelocity);
    calculatePulseARR = motorCharacter.freq;
    angular.calculateDegree = (motorCharacter.freq  * 360.0) / 10000.0;
    angular.timeForRevolution = taskDegree / angular.calculateDegree;
    if (FLAG_Revolution < angular.timeForRevolution)
    {
        Set_Smooth_Speed(taskAngularVelocity, arrTIM);
    }
    else
    {
        Set_Smooth_Speed(0, arrTIM);
    }
}

SSS_Parameters param;
void Set_Smooth_Speed(float speed, TIM_TypeDef *arrTIM)
{
    // static SSS_Parameters param;
    param.step = 60.0;
    param.setSpeed = param.previousValue;
    param.divStep = abs(speed - param.previousValue) / param.step;
    if (speed > param.previousValue + param.step)
    {
        for (uint8_t i = 0; i < param.divStep; i++)
        {
            delay(60);
            param.setSpeed += param.step;
            arrTIM->ARR = Speed_Transformation_In_ARR(param.setSpeed);
        }
    }
    else if (speed < param.previousValue - param.step)
    {
        for (uint8_t i = 0; i < param.divStep; i++)
        {
            delay(60);
            param.setSpeed -= param.step;
            arrTIM->ARR = Speed_Transformation_In_ARR(param.setSpeed);
        }
    }
    else if (speed == 0)
    {
        arrTIM->ARR = 0;
    }
    else
    {
        arrTIM->ARR = Speed_Transformation_In_ARR(speed);
    }
    param.setSpeed = 0;
    param.previousValue = speed;
    delay(5);
}