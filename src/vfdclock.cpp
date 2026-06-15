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

#define DIGIT_BCD_PORT			GPIOA
#define	DIGIT_BCD_PIN_A			GPIO_PIN_0 //A0
#define	DIGIT_BCD_PIN_B			GPIO_PIN_1 //A1
#define	DIGIT_BCD_PIN_C			GPIO_PIN_2 //A2
#define	DIGIT_BCD_PIN_D			GPIO_PIN_3 //A3
#define	DIGIT_BCD_PIN_CLK		GPIO_PIN_4 //A4
#define	DIGIT_BCD_PIN_COLON		GPIO_PIN_5 //A5
#define DIGIT_BCD_PIN_RESET		GPIO_PIN_6 //A6

#define DIGIT_PWM_PORT			GPIOB
#define DIGIT_PWM_PIN			GPIO_PIN_9 //B9

void SystemClock_Config(void);
void _Error_Handler(const char* file, int line);
void DigitPins_Init(void);
void DigitBCDReset(void);
void DigitBCDZero(void);
uint8_t decToBcd(uint8_t val);

int main(void) {
	HAL_Init();
	SystemClock_Config();
	DigitPins_Init();

	while (1) {
		DigitBCDReset();
		for (int i = 1; i <= 6; i++) {
			GPIOA->BSRR = DIGIT_BCD_PIN_CLK;
			HAL_Delay(1);
			GPIOA->BRR = 0xF;
			GPIOA->BSRR = (uint8_t)i;
			GPIOA->BRR = DIGIT_BCD_PIN_CLK;
			HAL_Delay(1);
		}
		HAL_Delay(5000);
		DigitBCDReset();
		DigitBCDZero();
		HAL_Delay(5000);
		HAL_GPIO_TogglePin(DIGIT_BCD_PORT, DIGIT_BCD_PIN_COLON);
		//HAL_Delay(1000);
	}
	return 0;
}

void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
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

void DigitPins_Init(void) {
	__HAL_RCC_GPIOA_CLK_ENABLE();

	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = DIGIT_BCD_PIN_A | DIGIT_BCD_PIN_B | 
						DIGIT_BCD_PIN_C | DIGIT_BCD_PIN_D | 
						DIGIT_BCD_PIN_CLK | DIGIT_BCD_PIN_COLON | DIGIT_BCD_PIN_RESET;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(DIGIT_BCD_PORT, &GPIO_InitStruct);
}

void DigitBCDReset(void) {
	GPIOA->BSRR = DIGIT_BCD_PIN_RESET;
	HAL_Delay(1);
	GPIOA->BRR = DIGIT_BCD_PIN_RESET;
	for (int i = 0; i <= 8; i++) {
		GPIOA->BSRR = DIGIT_BCD_PIN_CLK;
		HAL_Delay(1);
		GPIOA->BRR = DIGIT_BCD_PIN_CLK;
		HAL_Delay(1);
	}
}

void DigitBCDZero(void) {
	for (int i = 1; i <= 6; i++) {
			GPIOA->BSRR = DIGIT_BCD_PIN_CLK;
			HAL_Delay(1);
			GPIOA->BRR = 0xF;
			HAL_Delay(1);
			GPIOA->BRR = DIGIT_BCD_PIN_CLK;
			HAL_Delay(1);
		}
}

uint8_t decToBcd(uint8_t val)
{
  	return ((val/10 >> 4) + (val%10));
}

void _Error_Handler(const char* file, int line) {
	//Error occured so we did nothing
}

extern "C" void SysTick_Handler(void) {
	HAL_IncTick();
}