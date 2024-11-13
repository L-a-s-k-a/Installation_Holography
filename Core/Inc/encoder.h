#ifndef ENCODER_H
#define ENCODER_H

#include "init.h"
#include <stdlib.h>
#include <math.h>

typedef struct{
    float degreePerRound;
    float degreeCalculate;
    float radianPerRound;
    float radianCalculate;
}Encoder;

typedef struct{
    float angularSpeedInRPM;
    float angularSpeedInRadSec;
}SpeedCalculate;

extern Encoder *encoderCalculation;
extern SpeedCalculate *angularVelocity;

void Encoder_Degree_Transformations(uint16_t encoderValue, uint16_t oneRevolution);
void Encode_Angular_Speed_Calculation(void);

#endif 