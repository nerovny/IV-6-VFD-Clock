/********************************************************************************
  * @file     VFD Clock
  * @author   A. Nerovny
  * @brief    Under construction
  *******************************************************************************/
// TODO: 	LM2733YMFX/NOPB - 40В
// 			LMR64010XMF - 40v

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"
#include "stm32f1xx_hal_rcc.h"
#include "stm32f1xx_hal_rtc.h"
#include "stm32f1xx_hal_rtc_ex.h"
#include "clock_pins.h"
#include "clock_bcd.h" 
#include "ds1302.h"
#include "delay.h"

const char* DaysName[] = {0, "MON", "TUE", "WED", "THU", "FRI", "SAT", "SUN"};

void SystemClock_Config(void);
void _Error_Handler(const char* file, int line);

int main(void) {
	HAL_Init();
	SystemClock_Config();
	DigitPins_Init();
	HAL_Delay(10);

	DS1302_HandelTypeDef rtc = {
        .CE_Pin = {RTC_PORT, RTC_PIN_RESET},
        .IO_Pin = {RTC_PORT, RTC_PIN_DATA},
		.SCLK_Pin = {RTC_PORT, RTC_PIN_CLK}
    };

	// ONLY FOR TESTING PURPOSES // Set the initial date and time
    DS1302_TimeRecord datetime = {
        .sec = 10,
        .min = 23,
        .hour = {.hour=11, .meridiem=NONE}, // Set meridiem to NONE if using 24 hrs clock
        .date = 22,
        .month = 8,
        .year = 24,
        .day = TUE
    };

	RTCPins_Init();
	ds1302_init(&rtc);
	ds1302_setDateTime(&rtc, datetime);
	DigitBCDStartupRoll();

	DigitBCDReset();
	while (1) {
		DS1302_TimeRecord now = ds1302_getDateTime(&rtc);
		if (now.sec != datetime.sec) {
			datetime = ds1302_getDateTime(&rtc);
			DigitBCDPrint(datetime.hour.hour, datetime.min, datetime.sec);
			HAL_GPIO_TogglePin(DIGIT_BCD_PORT, DIGIT_BCD_PIN_COLON);
		}
		HAL_Delay(10);
	}
	return 0;
}

void SystemClock_Config(void) {

	__HAL_RCC_PWR_CLK_ENABLE();

	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_LSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
	RCC_OscInitStruct.LSIState = RCC_LSE_ON;
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
	// HAL_Delay(1) is 1ms
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

void _Error_Handler(const char* file, int line) {
	//Error occured so we did nothing
}

extern "C" void SysTick_Handler(void) {
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}