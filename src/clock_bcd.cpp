/*
*   @file   clock_bcd.h
*   @brief  BCD handling definitions and functions header file
*/

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"
#include "clock_pins.h"
#include "clock_bcd.h"
#include "delay.h"

void DigitBCDReset(void) {
	GPIOA->BSRR = DIGIT_BCD_PIN_RESET;
	Delay_us(100);
	GPIOA->BRR = DIGIT_BCD_PIN_RESET;
	for (int i = 0; i <= 8; i++) {
		GPIOA->BSRR = DIGIT_BCD_PIN_CLK;
		Delay_us(100);
		GPIOA->BRR = DIGIT_BCD_PIN_CLK;
		Delay_us(100);
	}
}

void DigitBCDZero(void) {
	for (int i = 1; i <= 6; i++) {
			GPIOA->BSRR = DIGIT_BCD_PIN_CLK;
			Delay_us(100);
			GPIOA->BRR = DIGIT_BYTE_CLEAN;
			Delay_us(100);
			GPIOA->BRR = DIGIT_BCD_PIN_CLK;
			Delay_us(100);
		}
}

void DigitBCDPrint(unsigned int num_high, unsigned int num_middle, unsigned int num_low) {
	uint8_t high_byte = INT_TO_BCD_8BIT(num_high);
	uint8_t mid_byte = INT_TO_BCD_8BIT(num_middle);
	uint8_t low_byte = INT_TO_BCD_8BIT(num_low);

	DigitBCDReset();
	DigitBCDPrintByte(low_byte);
	DigitBCDPrintByte(mid_byte);
	DigitBCDPrintByte(high_byte);
}

void DigitBCDPrintByte(uint8_t bcd_byte) {
	GPIOA->BSRR = DIGIT_BCD_PIN_CLK;
	Delay_us(100);
	GPIOA->BRR = DIGIT_BYTE_CLEAN;
	Delay_us(100);
	GPIOA->BSRR = bcd_byte & 0x0F; //extract lower nibble by the 0x00001111 mask
	Delay_us(100);
	GPIOA->BRR = DIGIT_BCD_PIN_CLK;
	Delay_us(100);
	GPIOA->BSRR = DIGIT_BCD_PIN_CLK;
	Delay_us(100);
	GPIOA->BRR = DIGIT_BYTE_CLEAN;
	Delay_us(100);
	GPIOA->BSRR = (bcd_byte >> 4) & 0x0F; //extract higher nibble
	Delay_us(100);
	GPIOA->BRR = DIGIT_BCD_PIN_CLK;
	Delay_us(100);
}

void DigitBCDStartupRoll(void) {
	DigitBCDReset();
	GPIOA->BSRR = DIGIT_BCD_PIN_COLON;
	for (int i = 0; i <= 99; i++) {
		DigitBCDPrint(i, i, i);
		HAL_Delay(10);
	}
}