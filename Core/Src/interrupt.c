#include "../Inc/interrupt.h"
#include "../Inc/motorDriver.h"

uint16_t TickDelayCount, buttonTickCount;
uint16_t ENC_CNT, encItr;
uint8_t FLAG_Delay, encDir;
uint32_t TIMCount, calculateDeg;

void SysTick_Handler(void)
{
    buttonTickCount++;
    GlobalTickCount++;
    if (FLAG_Delay)
        TickDelayCount++;
    ENC_CNT = TIM2->CNT;
    encDir = READ_BIT(TIM2->CR1, TIM_CR1_DIR) >> 4;
}

void delay(int del)
{
    FLAG_Delay = 1;
    while (TickDelayCount < del)
    {
    }
    TickDelayCount = 0;
    FLAG_Delay = 0;
}

void EXTI15_10_IRQHandler(void)
{
    SET_BIT(EXTI->PR, EXTI_PR_PR13);
    if (buttonTickCount > 100)
    {
        buttonTickCount = 0;
        BtnNum = !BtnNum;
    }
}

void TIM3_IRQHandler(void)
{
    if (READ_BIT(TIM3->SR, TIM_SR_UIF))
    {
        TIMCount += 10;
        if (TIMCount >= calculatePulseARR)
        {
            FLAG_Revolution += 0.1;
            TIMCount = 0;
        }
        CLEAR_BIT(TIM3->SR, TIM_SR_UIF);
    }
}

void TIM2_IRQHandler(void)
{
    if (TIM2->SR & TIM_SR_UIF)
    {
        if (READ_BIT(TIM2->CR1, TIM_CR1_DIR) != 0)
        {
            encItr--;
        }
        else
            encItr++;
        CLEAR_BIT(TIM2->SR, TIM_SR_UIF);
    }
}