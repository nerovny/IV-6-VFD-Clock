/*
*   @file   clock_rtc.h
*   @brief  STM32F103 internal RTC module handler header
*/

#include <stdbool.h>
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"

extern bool IS_TIME_UPDATED;

RTC_HandleTypeDef hrtc = {0};
RTC_TimeTypeDef sTime = {0};
RTC_DateTypeDef sDate = {0};

void MX_RTC_Init(void);
