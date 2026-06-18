/*
*   @file   clock_rtc.cpp
*   @brief  STM32F103 internal RTC module handler
*/


#include "clock_rtc.h"

bool IS_TIME_UPDATED = false;

RTC_HandleTypeDef hrtc = {0};
RTC_TimeTypeDef sTime = {0};
RTC_DateTypeDef sDate = {0};

void MX_RTC_Init(void);


void MX_RTC_Init(void)
{
	__HAL_RCC_BKP_CLK_ENABLE();
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_RCC_BKP_FORCE_RESET();
	__HAL_RCC_BKP_RELEASE_RESET();
	__HAL_RCC_RTC_ENABLE();

	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

	// Select the RTC clock source
	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
	PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
	HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

	// Initialize RTC instance
	hrtc.Instance = RTC;
	hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
	hrtc.Init.OutPut = RTC_OUTPUTSOURCE_NONE;
	HAL_RTC_Init(&hrtc);

	// Enable RTC global interrupt through NVIC
    HAL_NVIC_SetPriority(RTC_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(RTC_IRQn);

	// Enable the RTC Second Interrupt
	__HAL_RTC_SECOND_ENABLE_IT(&hrtc, RTC_IT_SEC);

    HAL_RTC_WaitForSynchro(&hrtc);

    // Initialize RTC and set the Time and Date 
	sTime.Hours = 10;
	sTime.Minutes = 15;
	sTime.Seconds = 0;
	HAL_PWR_EnableBkUpAccess();
	HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	HAL_PWR_DisableBkUpAccess();
	sDate.Date = 1;
	sDate.Month = 1;
	sDate.Year = 5;
	HAL_PWR_EnableBkUpAccess();
	HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
	HAL_PWR_DisableBkUpAccess();
}

extern "C" void RTC_IRQHandler(void) {
    // Check for RTC Second Interrupt
    if ((RTC->CRL & RTC_CRL_SECF) != RESET) {
        IS_TIME_UPDATED = true; // Set the interrupt flag
        CLEAR_BIT(RTC->CRL, RTC_CRL_SECF); // Clear the Second Interrupt
    }
}

