#ifndef MOTORDRIVER_H
#define MOTORDRIVER_H

#include "../Inc/init.h"
#include <stdlib.h>
#include <math.h>

typedef struct
{
    float FCPU;
    float freqMax;
    float freq;
    float PPS; //Pulse Per Second
    float oneRPS; //One revolution per second
    float revPSMax;
    float revPMMax;
    float divisionFactor;
    float radPSMax;
    float radPMMax;
    float revPS;
} Motor_Characteristics;

typedef struct
{
    float previousValue;
    float step;
    float divStep;
    float setSpeed;
} SSS_Parameters;

typedef struct
{
    float angularDivision;
    float angularSpeed;
    float timeForRevolution;
    float calculateDegree;
} Angular_Parameters;

typedef enum
{
    REV_PER_MIN,
    REV_PER_SEC,
    RAD_PER_MIN,
    RAD_PER_SEC
} Calc_Mode;

extern float calculatePulseARR;

void Motor_Init(float rpmMax, float freqMax, float rpsMax, Calc_Mode mode);
float Speed_Transformation_In_ARR(float value);
void Set_Number_Of_Revolutions(float revolution, float taskAngularVelocity, TIM_TypeDef *arrTIM);
void Set_Smooth_Speed(float speed, float step, TIM_TypeDef *arrTIM);

#endif