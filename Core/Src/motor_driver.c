#include <motot_driver.h>

float Set_Speed(float rpm)
{
    Parameters param;
    param.FCPU = 180000000;
    param.maxFreq = 500000;
    param.rpsMax = 50;
    param.coef = (param.FCPU * param.rpsMax * 60) / param.maxFreq;

    return param.coef / rpm;
    // Soft_Start(rpm);
    // return param.FCPU / ((param.maxFreq * ((float)(rpm) / 60.0)) / param.rpsMax);
}

int previousValue = 0;
uint8_t steps = 52, divStep;
volatile uint16_t setSpeed;

void Soft_Start(int speed)
{
    if (speed > previousValue + steps)
    {
        divStep = speed / steps;
        for (uint8_t i = 0; i < divStep; i++)
        {
            delay(100);
            setSpeed += steps;
            TIM3->ARR = Set_Speed(setSpeed);
        }
        // setSpeed = 0;
    }
    else if(speed < previousValue){

    }
    TIM3->ARR = Set_Speed(speed);
    previousValue = speed;
}