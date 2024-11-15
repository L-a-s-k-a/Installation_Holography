#include "../Inc/init.h"
#include "../Inc/interrupt.h"
#include "../Inc/motorDriver.h"
#include "../Inc/encoder.h"

uint8_t BtnNum;
uint16_t GlobalTickCount;
uint16_t ARRView;
float FLAG_Revolution;
float calculatePulseARR;
float speed = 1.0, revolution = 1.0;

int main(void)
{
    SysTick_Init();
    RCC_Init();
    GPIO_Init();
    ITR_init();
    TIM_PWM_Init();
    TIM_ENCODER_Init();
    SystemInit();

    Motor_Init(3000.0, 500000.0, 50.0, REV_PER_SEC);

    while (1)
    {
        ARRView = TIM3->ARR;
        Set_Number_Of_Revolutions(revolution, speed, TIM3);
        // Set_Smooth_Speed(speed, REV_PER_MIN);
    }
}