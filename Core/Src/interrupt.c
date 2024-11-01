#include "interrupt.h"

// extern uint16_t GlobalTickCount, MotorTickCount; // extern - обозначает, что переменная будет объявленна в другом месте
extern uint8_t BtnNum, TIMCount;
uint16_t TickDelayCount, ARRView;
uint8_t FLAG_Delay;

void SysTick_Handler(void)
{
    // TickDelayCount++;
    GlobalTickCount++;
    if (FLAG_Delay) TickDelayCount++;
    ARRView = TIM3->ARR;
}

void delay(int del){
    FLAG_Delay = 1;
    while(TickDelayCount < del){}
    TickDelayCount = 0;
    FLAG_Delay = 0;
}

void EXTI15_10_IRQHandler(void)
{

    SET_BIT(EXTI->PR, EXTI_PR_PR13);

    if (TickDelayCount > 100)
    {
        BtnNum++;
        if (BtnNum >= 5) BtnNum = 0;
    }
}


void TIM3_IRQHandler(void){
    if (READ_BIT(TIM3->SR, TIM_SR_CC1IF)){
        CLEAR_BIT(TIM3->SR, TIM_SR_CC1IF);
    }
    TIMCount++;
}