#include <init.h>
#include <stdlib.h>
#include <math.h>

typedef struct
{
    float FCPU;
    float freqMax;
    float revPSMax;
    float revPMMax;
    float divisionFactor;
    float radPSMax;
    float radPMMax;
} MotorCharacteristics;

typedef struct
{
    float previousValue;
    float step;
    float divStep;
    float setSpeed;
} SCS_Parameters;

typedef enum
{
    REV_PER_MIN,
    REV_PER_SEC,
    RAD_PER_MIN,
    RAD_PER_SEC
} SCS_Mode;

void Motor_Init(float rpmMax, float freqMax, float rpsMax);
float Speed_Transformations(float value, uint8_t mode);
void Smooth_Change_Speed(float speed, uint8_t mode);