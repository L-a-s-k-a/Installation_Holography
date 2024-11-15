#ifndef INTERRUPT_H
#define INTERRUPT_H

#include "init.h"

extern uint16_t GlobalTickCount, MotorTickCount;
extern uint16_t ENC_CNT, encItr;
extern uint8_t encDir, BtnNum;
extern float FLAG_Revolution;

void EXTI15_10_IRQHandler(void);
void SysTick_Handler(void);
void TIM3_IRQHandler(void);
void TIM2_IRQHandler(void);
void delay(int del);

#endif