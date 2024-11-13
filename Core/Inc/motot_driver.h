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

typedef struct
{
    float angularDivision;
    float angularSpeed;
} Angular_Parameters;

typedef enum
{
    REV_PER_MIN,
    REV_PER_SEC,
    RAD_PER_MIN,
    RAD_PER_SEC
} SCS_Mode;

MotorCharacteristics *motorCharacter;
Angular_Parameters *angular;

void Motor_Init(float rpmMax, float freqMax, float rpsMax);
float Speed_Transformations(float value, SCS_Mode mode);
void Angle_Transformations(float value);
void Smooth_Change_Speed(float speed, SCS_Mode mode);