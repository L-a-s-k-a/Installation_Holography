#include "../Inc/init.h"
#include "../Inc/interrupt.h"
#include <motot_driver.h>

uint8_t BtnNum, TIMCount;
uint16_t GlobalTickCount;

float speed = 60, softStart;
int test;

int main(void)
{
    SysTick_Init();
    RCC_Init();
    GPIO_Init();
    ITR_init();
    TIM_Init();
    SystemInit();

    while (1)
    {
        // delay(1000);
        // SET_BIT(GPIOB->BSRR, GPIO_BSRR_BS7);
        // delay(1000);
        // SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR7);
        // test++;
        
        // test = Set_Speed(speed * 2);
        Soft_Start(speed * 2);
        // softStart = Soft_Start(speed * 2);
        

        // TIM3->ARR = Set_Speed(speed * 2);
        // TIM3->CCR1 = Set_Speed(speed);
    }
}