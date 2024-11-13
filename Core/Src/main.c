#include "../Inc/init.h"
#include "../Inc/interrupt.h"
#include "motot_driver.h"
#include "encoder.h"

uint8_t BtnNum;
uint16_t GlobalTickCount, TIMCount;
uint8_t FLAG_Revolution;
uint16_t ARRView;

Encoder *encoderCalculation;
SpeedCalculate *angularVelocity;

float speed = 60.0;

int main(void)
{
    SysTick_Init();
    RCC_Init();
    GPIO_Init();
    ITR_init();
    TIM_PWM_Init();
    TIM_ENCODER_Init();
    SystemInit();

    Motor_Init(3000.0, 500000.0, 50.0);

    while (1)
    {
        ARRView = TIM3->ARR;
        if(BtnNum) FLAG_Revolution = 0;
        if (FLAG_Revolution <= 2){
            Smooth_Change_Speed(speed, REV_PER_MIN);
        }
        else Smooth_Change_Speed(0, REV_PER_MIN);
        // Smooth_Change_Speed(0, REV_PER_MIN);
        // Angle_Transformations(360.0);
    }
}