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
uint8_t alarm = 0;

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
        if(alarm) SET_BIT(GPIOB->BSRR, GPIO_BSRR_BS0);
        else SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR0);
        Set_Number_Of_Revolutions(revolution, speed, TIM3);
        Set_Smooth_Speed(speed, TIM3);
    }
}