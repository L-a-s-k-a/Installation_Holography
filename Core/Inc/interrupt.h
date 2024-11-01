#include "init.h"

extern uint16_t GlobalTickCount, MotorTickCount;

void EXTI15_10_IRQHandler(void);
void SysTick_Handler(void);
void TIM1_UP_TIM10_IRQHandler(void);
void TIM3_IRQHandler(void);
void delay(int del);
