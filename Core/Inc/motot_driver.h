#include <init.h>
#include <stdlib.h>
#include <math.h>


#define REVOLUTIONS_PER_MINUTE
// #define RADIANS_PER_MINUTE
// #define REVOLUTIONS_PER_SECOND
// #define RADIANS_PER_SECOND
// #define FREQUENCY

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
    CHECK
} MotorDriverConst;

void Motor_Init(float rpmMax, float freqMax, float rpsMax);
float Speed_Transformations(float value, char *mode);
void Smooth_Change_Speed(float speed);