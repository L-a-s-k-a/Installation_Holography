#include "init.h"

void SysTick_Init(void){
    CLEAR_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk); //На всякий предварительно выключим счётчик
	SET_BIT(SysTick->CTRL, SysTick_CTRL_TICKINT_Msk); //Разрешаем прерывание по системному таймеру
	SET_BIT(SysTick->CTRL, SysTick_CTRL_CLKSOURCE_Msk); //Источник тактирования будет идти из AHB без деления
	MODIFY_REG(SysTick->LOAD, SysTick_LOAD_RELOAD_Msk,
			179999 << SysTick_LOAD_RELOAD_Pos); //Будет тикать с частотой 1 кГц и вызывать прерывания
	MODIFY_REG(SysTick->VAL, SysTick_VAL_CURRENT_Msk,
			179999 << SysTick_VAL_CURRENT_Pos); //Начнём считать со значения 179 999
	SET_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk); //Включим счётчик
}

void RCC_Init(void){
    /* Настройка мощности.
     * Включение тактирвоания мощности. 
     * Включение регулятора напряжения на выходе (VOS биты регистра PWR_CR)
     * Включение овер-драйва
     * Включение переключателя овер-драйва
    */
    // SET_BIT(RCC->APB1ENR, RCC_APB1ENR_PWREN);
	// while (READ_BIT(RCC->APB1ENR, RCC_APB1ENR_PWREN) == RESET);
	// MODIFY_REG(PWR->CR, PWR_CR_VOS, PWR_CR_VOS);
	// while (READ_BIT(PWR->CR, PWR_CR_VOS) == RESET);
	// SET_BIT(PWR->CR, PWR_CR_ODEN);
	// while (READ_BIT(PWR->CR, PWR_CSR_ODRDY) == RESET);
	// SET_BIT(PWR->CR, PWR_CR_ODSWEN);
	// while (READ_BIT(PWR->CR, PWR_CSR_ODSWRDY) == RESET);



    // SET_BIT(RCC->CR, RCC_CR_HSION); //Включаем внутренний резонатор
    // while(READ_BIT(RCC->CR, RCC_CR_HSIRDY) == RESET); //Ждём пока он включится

    /* Предварительная очистка регистров RCC */
	MODIFY_REG(RCC->CR, RCC_CR_HSITRIM, 0x80U);
	CLEAR_REG(RCC->CFGR);
	while(READ_BIT(RCC->CFGR, RCC_CFGR_SWS) != RESET);
	CLEAR_BIT(RCC->CR, RCC_CR_PLLON);
	while (READ_BIT(RCC->CR, RCC_CR_PLLRDY) != RESET);
	CLEAR_BIT(RCC->CR, RCC_CR_HSEON | RCC_CR_CSSON);
	while (READ_BIT(RCC->CR, RCC_CR_HSERDY) != RESET);
	CLEAR_BIT(RCC->CR, RCC_CR_HSEBYP);

    /* Настройка главного регистра RCC */
    SET_BIT(RCC->CR, RCC_CR_HSEON); //Запускаем внешний кварцевый резонатор
    while(READ_BIT(RCC->CR, RCC_CR_HSERDY) == RESET); //Ждём пока он запустится
    CLEAR_BIT(RCC->CR, RCC_CR_HSEBYP); //Сбросим бит байпаса в 0, если вдруг там что-то лежит
    SET_BIT(RCC->CR, RCC_CR_CSSON); //Запустим Clock detector
    
    /* Натсройка регистров PLL
     * Предварительная очистка регистра
     * В качестве источника тактирования для PLL выбирается HSE
     * Мы сначала делим входную частоту (HSE) на 4 (получаем 2 МГц), затем умножем на 180 и снова делим на 2, таким образом получем 180МГц
     * Включаем работу PLL
     */
    CLEAR_REG(RCC->PLLCFGR);
    SET_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLSRC_HSE);
    MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLM, RCC_PLLCFGR_PLLM_2); //Выставляем предделитель входной частоты PLL на 4 
    MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLN_Msk, RCC_PLLCFGR_PLLN_2 | RCC_PLLCFGR_PLLN_4 | RCC_PLLCFGR_PLLN_5 | RCC_PLLCFGR_PLLN_7);
    CLEAR_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLP_Msk); //Настраиваем предделитель получившейся частоты после умножения. Иными словами, получаем итоговую частоту PLL
    // MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLQ, RCC_PLLCFGR_PLLQ_2); //Предделитель частоты для USB OTG FS, SDIO и генератора рандомных чисел настраиваем на 48МГц (Максимальная возможная частота 48МГц)
    
    SET_BIT(RCC->CR, RCC_CR_PLLON); //Запустим PLL
    while(READ_BIT(RCC->CR, RCC_CR_PLLRDY)); //Ждём запуска PLL

    /* Настройка основных конфигураций RCC
     * В качетсве системных часов выбираем выход PLL
     * Настраиваем предделители шин AHB и APB
     * Настраиваем выходы MCO1 и MCO2 для внешней оценки настроенной системы тактирования
     */
    MODIFY_REG(RCC->CFGR, RCC_CFGR_SW, RCC_CFGR_SW_PLL); //Выбираем PLL в качестве System Clock
    MODIFY_REG(RCC->CFGR, RCC_CFGR_HPRE, RCC_CFGR_HPRE_DIV1); //Предделитель AHB, без делителя
    MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE1, RCC_CFGR_PPRE1_DIV1); //Предделитель APВ1, делим на 4
    MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE2, RCC_CFGR_PPRE2_DIV1); //Предделитель APВ2, делим на 2
    MODIFY_REG(RCC->CFGR, RCC_CFGR_MCO2PRE, RCC_CFGR_MCO2PRE_Msk); //Предделитель на выходе MCO2 (PC9) = 5
    CLEAR_BIT(RCC->CFGR, RCC_CFGR_MCO2); //Настраиваем на выход MCO2 - System clock

    /* Настройка задержки внутренней памяти
     * Выставление битов LATENCY регистра FLASH_ACR в позицию 5SW (6 CPU cycles). 
     * Данная настройка необходима при увеличении системной частоты тактирования свыше 20 МГц. Таблица 12 RM0090
    */
    MODIFY_REG(FLASH->ACR, FLASH_ACR_LATENCY, FLASH_ACR_LATENCY_5WS);
}

void GPIO_Init(void){
    /*Включение тактирования GPIO*/
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; //Включение тактирования GPIO порта B
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN; //Включение тактирования GPIO порта C
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    SET_BIT(GPIOB->MODER, GPIO_MODER_MODER7_0 | GPIO_MODER_MODER0_0);
    CLEAR_BIT(GPIOB->OTYPER, GPIO_OTYPER_OT7 | GPIO_OTYPER_OT0);
    SET_BIT(GPIOB->OSPEEDR, GPIO_OSPEEDR_OSPEED7_0 | GPIO_OSPEEDR_OSPEED0_0);
    SET_BIT(GPIOB->ODR, GPIO_ODR_OD7 | GPIO_ODR_OD0);
    SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR7 | GPIO_BSRR_BR0);

    SET_BIT(GPIOB->MODER, GPIO_MODER_MODER14_0);
    CLEAR_BIT(GPIOB->OTYPER, GPIO_OTYPER_OT14);
    SET_BIT(GPIOB->OSPEEDR, GPIO_OSPEEDR_OSPEED14_0);
    SET_BIT(GPIOB->ODR, GPIO_ODR_OD14);
    SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR14);

    // Настройка порта РС13 на вход со стяжкой (Pull-down)
    CLEAR_BIT(GPIOC->MODER, GPIO_MODER_MODER13_0);
    SET_BIT(GPIOC->PUPDR, GPIO_PUPDR_PUPD13_1);
 
    SET_BIT(GPIOC->MODER, GPIO_MODER_MODER9_1);
    CLEAR_BIT(GPIOC->OTYPER, GPIO_OTYPER_OT9); 
    SET_BIT(GPIOC->OSPEEDR, GPIO_OSPEEDR_OSPEED9_Msk);
    CLEAR_BIT(GPIOC->PUPDR, GPIO_PUPDR_PUPD9);
    MODIFY_REG(GPIOC->AFR[1], GPIO_AFRH_AFSEL9_Msk, 0x0);

    SET_BIT(GPIOA->MODER, GPIO_MODER_MODER6_1);
    SET_BIT(GPIOA->OSPEEDR, GPIO_OSPEEDR_OSPEED6_0);
    MODIFY_REG(GPIOA->AFR[0], GPIO_AFRL_AFSEL6_Msk, GPIO_AFRL_AFSEL6_1);
}

void ITR_init(void){
    SET_BIT(RCC->APB2ENR, RCC_APB2ENR_SYSCFGEN); //Включение тактирования периферии SYSCFG

    MODIFY_REG(SYSCFG->EXTICR[3], SYSCFG_EXTICR4_EXTI13_Msk, SYSCFG_EXTICR4_EXTI13_PC); //Настройка мультиплексора на вывод линии прерывания EXTI13 на PC13
    SET_BIT(EXTI->IMR, EXTI_IMR_MR13); //Настройка маскирования 13 линии
    SET_BIT(EXTI->RTSR, EXTI_RTSR_TR13); //Настройка детектирования нарастающего фронта 13 линии
    CLEAR_BIT(EXTI->FTSR, EXTI_FTSR_TR13); //Настройка детектирования спадающего фронта 13 линии

    NVIC_SetPriority(EXTI15_10_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0)); //Установка 0 приоритета прерывания для вектора EXTI15_10
    NVIC_EnableIRQ(EXTI15_10_IRQn); //Включение прерывания по вектору EXTI15_10
}

void TIM_Init(void){
    SET_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM3EN);

    TIM3->PSC = 1-1;
    TIM3->ARR = 360-1;

    SET_BIT(TIM3->CR1, TIM_CR1_CEN);

    SET_BIT(TIM3->DIER, TIM_DIER_CC1IE);

    CLEAR_BIT(TIM3->CCMR1, TIM_CCMR1_CC1S_Msk);
    // SET_BIT(TIM3->CCMR1, TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1); // PWM generation
    SET_BIT(TIM3->CCMR1, TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_0); // OC1REF Toggles when TIM3_CNT=TIM3_CCR1.

    SET_BIT(TIM3->CCER, TIM_CCER_CC1E); 
    SET_BIT(TIM3->CCER, TIM_CCER_CC1P);

    // TIM3->CCR1 = 180;
}