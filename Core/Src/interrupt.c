#include "interrupt.h"
#include "encoder.h"
#include "motot_driver.h"

uint16_t TickDelayCount, buttonCount;
uint16_t ENC_CNT, encItr;
uint8_t FLAG_Delay, encDir;

void SysTick_Handler(void)
{
    buttonCount++;
    GlobalTickCount++;
    if (FLAG_Delay)
        TickDelayCount++;
    ENC_CNT = TIM2->CNT;
    encDir = READ_BIT(TIM2->CR1, TIM_CR1_DIR) >> 4;
    // Encoder_Degree_Transformations(ENC_CNT, encDir);
    // Encode_Angular_Speed_Calculation();
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
    if (buttonCount > 100)
    {
        buttonCount = 0;
        BtnNum = !BtnNum;
    }
}

void TIM3_IRQHandler(void)
{
    if (READ_BIT(TIM3->SR, TIM_SR_UIF))
    {
        TIMCount++;
        if (TIMCount >= 10000)
        {
            FLAG_Revolution++;
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