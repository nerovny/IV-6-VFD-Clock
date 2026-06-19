#include "stm32f1xx_hal.h"
#include "delay.h"

// Функция инициализации счетчика тактов
void DelayUS_Init(void) {
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

// Функция задержки в микросекундах
void Delay_us(uint32_t us) {
    uint32_t start_tick = DWT->CYCCNT;
    // SystemCoreClock содержит частоту процессора в Гц
    uint32_t ticks_per_us = SystemCoreClock / 1000000;
    uint32_t delay_ticks = us * ticks_per_us;
    
    while ((DWT->CYCCNT - start_tick) < delay_ticks);
}