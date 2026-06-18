/**
  ******************************************************************************
  * @file     VFD Clock
  * @author   A. Nerovny
  * @brief    Under construction
  ******************************************************************************
  */

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"
#include "stm32f1xx_hal_rcc.h"
#include "stm32f1xx_hal_rtc.h"
#include "stm32f1xx_hal_rtc_ex.h"
#include "clock_pins.h"
#include "clock_bcd.h"
#include "clock_rtc.h" 

void SystemClock_Config(void);
void _Error_Handler(const char* file, int line);

int main(void) {
	HAL_Init();
	SystemClock_Config();
	DigitPins_Init();
	HAL_Delay(100);
	MX_RTC_Init();

	DigitBCDPrint(12, 34, 95);
	GPIOA->BSRR = DIGIT_BCD_PIN_COLON;

	while (1) {
		if (IS_TIME_UPDATED) {
			HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
			HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
			
			HAL_GPIO_TogglePin(DIGIT_BCD_PORT, DIGIT_BCD_PIN_COLON);
			IS_TIME_UPDATED = false;
		}
	}
	return 0;
}

void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_LSI;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.LSIState = RCC_LSI_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;

	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
  	}

	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
				|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, 
				FLASH_LATENCY_2) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	//Configure the SysTick frequency, clock source and interrupt priority.
	// HAL_Delay(1) is 100ns
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/10000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
	HAL_NVIC_SetPriority(SysTick_IRQn, 15, 0);
}

void _Error_Handler(const char* file, int line) {
	//Error occured so we did nothing
}

extern "C" void SysTick_Handler(void) {
	HAL_IncTick();
}