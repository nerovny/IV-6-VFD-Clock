/*
*   @file   clock_rtc.h
*   @brief  STM32F103 internal RTC module handler header
*/

#include <stdbool.h>
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"

extern bool IS_TIME_UPDATED;

extern RTC_HandleTypeDef hrtc;
extern RTC_TimeTypeDef sTime;
extern RTC_DateTypeDef sDate;

void MX_RTC_Init(void);
