#include "encoder.h"

Encoder *encoderCalculation;
SpeedCalculate *angularVelocity;

void Encoder_Degree_Transformations(uint16_t encoderValue, uint16_t oneRevolution){
    encoderCalculation->degreePerRound = encoderValue / 360.0;
    encoderCalculation->degreeCalculate = oneRevolution * 360.0 + encoderCalculation->degreePerRound;
    encoderCalculation->radianPerRound = encoderCalculation->degreePerRound * 2 * M_PI;
    encoderCalculation->radianCalculate = encoderCalculation->degreeCalculate * 2 * M_PI;
}

void Encode_Angular_Speed_Calculation(void){
    static float degLastState;
    static float difference;    
    difference = encoderCalculation->degreePerRound - degLastState;
    angularVelocity->angularSpeedInRPM = (difference / 0.1) * 10.0 * 60.0;
    degLastState = encoderCalculation->degreePerRound;
}