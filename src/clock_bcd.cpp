/*
*   @file   clock_bcd.h
*   @brief  BCD handling definitions and functions header file
*/

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"
#include "clock_pins.h"
#include "clock_bcd.h"

void DigitBCDReset(void);
void DigitBCDZero(void);

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
			GPIOA->BRR = DIGIT_BYTE_CLEAN;
			HAL_Delay(1);
			GPIOA->BRR = DIGIT_BCD_PIN_CLK;
			HAL_Delay(1);
		}
}