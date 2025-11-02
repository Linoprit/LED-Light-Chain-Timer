/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2023/12/25
 * Description        : Main program body.
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

/*
 *@Note
 *CH32V003F4P6 - Interrupts
 *https://curiousscientist.tech/blog/ch32v003f4p6-interrupts
 */

#include "main.h"
#include "stdbool.h"

void countToTarget(uint32_t target);

void initializeTimer(uint16_t prsc, uint16_t arr)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure = {0};
    NVIC_InitTypeDef NVIC_InitStructure = {0};

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    TIM_TimeBaseStructure.TIM_Period = arr;
    TIM_TimeBaseStructure.TIM_Prescaler = prsc;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update),
        TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM_Cmd(TIM2, ENABLE);
}

#ifdef USE_UART_60MIN
const uint32_t targetPause = 3600 * 1;
const uint32_t targetActive = 3600 * 1;
#else
const uint32_t targetPause = 3600 * 18;
const uint32_t targetActive = 3600 * 6;
#endif

uint32_t actCount = 0;

#ifdef USE_UART_1MIN
bool done = false;
#endif

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

    SystemCoreClockUpdate();
    Delay_Init();
    Delay_Ms(1000);
    GPIOConfig();

#ifdef F4P6
    DoInitSequence();
#endif

#if defined(USE_UART_1MIN) || defined(USE_UART_60MIN)
    USART_Printf_Init(115200);
    printf("CH32V003 - Timer test\n");
#endif

#ifdef CRYSTAL
    initializeTimer(2666, 999); // 1kHz, 1s period
#endif
#ifdef BUILDIN_RC
    initializeTimer(8049, 999); // 1kHz, 1s period
#endif

    while (1)
    {
#ifdef USE_UART_60MIN
        printf("A\n");
#endif

        GPIO_WriteBit(LED_FAIRY_PORT, LED_FAIRY_PIN, BLINKY_ON);
        countToTarget(targetActive);

#ifdef F4P6
        for (int i = GPIO_Pin_1; i < GPIO_Pin_7; i++)
            GPIO_WriteBit(LED_FAIRY_PORT, i, BLINKY_OFF);
#endif

#ifdef USE_UART_60MIN
        printf("B\n");
        printf("A\n");
#endif

        GPIO_WriteBit(LED_FAIRY_PORT, LED_FAIRY_PIN, BLINKY_OFF);
        countToTarget(targetPause);

#ifdef F4P6
        for (int i = GPIO_Pin_1; i < GPIO_Pin_7; i++)
            GPIO_WriteBit(LED_FAIRY_PORT, i, BLINKY_OFF);
#endif

#ifdef USE_UART_60MIN
        printf("B\n");
#endif
    }
}

void countToTarget(uint32_t target)
{
#ifdef F4P6
    const uint32_t fraction = target / 6;
#endif

#ifdef USE_UART_1MIN
    const uint32_t moduloCount = 60;
    printf("A\n");
#endif

    actCount = 0;
    while (actCount < target)
    {
#ifdef F4P6
        if (actCount >= fraction)
            GPIO_WriteBit(LED_FAIRY_PORT, GPIO_Pin_1, BLINKY_ON);
        if (actCount >= fraction * 2)
            GPIO_WriteBit(LED_FAIRY_PORT, GPIO_Pin_2, BLINKY_ON);
        if (actCount >= fraction * 3)
            GPIO_WriteBit(LED_FAIRY_PORT, GPIO_Pin_3, BLINKY_ON);
        if (actCount >= fraction * 4)
            GPIO_WriteBit(LED_FAIRY_PORT, GPIO_Pin_4, BLINKY_ON);
        if (actCount >= fraction * 5)
            GPIO_WriteBit(LED_FAIRY_PORT, GPIO_Pin_5, BLINKY_ON);
        if (actCount >= fraction * 6 - 2)
            GPIO_WriteBit(LED_FAIRY_PORT, GPIO_Pin_6, BLINKY_ON);
#endif

        Delay_Ms(5);

#ifdef USE_UART_1MIN
        if (((actCount % moduloCount) == 0) && done == false)
        {
            done = true;
            printf("B\n");
            printf("A\n");
        }
        if ((actCount % moduloCount) != 0)
        {
            done = false;
        }
#endif

        // PWR_EnterSTANDBYMode(PWR_STANDBYEntry_WFE);
    }
}

void TIM2_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        actCount++;

#ifdef F4P6
        GPIO_WriteBit(BLINKY_GPIO_PORT, BLINKY_GPIO_PIN,
                      (GPIO_ReadOutputDataBit(BLINKY_GPIO_PORT, BLINKY_GPIO_PIN) == Bit_SET) ? Bit_RESET : Bit_SET);
#endif
    }
}
