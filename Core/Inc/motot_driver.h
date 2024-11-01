#include <init.h>
#include "interrupt.h"

typedef struct{
    float FCPU;
    float maxFreq;
    float rpsMax;
    float coef;
} Parameters;

typedef enum{
    CHECK
} MotorDriverConst;

// uint16_t MotorTickCount;

float Set_Speed(float rpm);
void Smooth_Change_Speed(int speed);