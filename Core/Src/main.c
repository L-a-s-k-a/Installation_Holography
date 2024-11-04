#include "../Inc/init.h"
#include "../Inc/interrupt.h"
#include <motot_driver.h>

uint8_t BtnNum, TIMCount;
uint16_t GlobalTickCount;

float speed = 60.0;

int main(void)
{
    SysTick_Init();
    RCC_Init();
    GPIO_Init();
    ITR_init();
    TIM_Init();
    SystemInit();

    Motor_Init(3000.0, 500000.0, 50.0);

    while (1)
    {
        Smooth_Change_Speed(speed, REV_PER_MIN);
    }
}