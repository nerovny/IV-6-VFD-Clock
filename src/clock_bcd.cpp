/*
*   @file   clock_bcd.h
*   @brief  BCD handling definitions and functions header file
*/

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"
#include "clock_pins.h"
#include "clock_bcd.h"
#include "delay.h"
#include "buzzer.h"

void DigitBCDReset(void) {
	GPIOA->BSRR = DIGIT_BCD_PIN_RESET;
	Delay_us(DIGIT_DELAY_US);
	GPIOA->BRR = DIGIT_BCD_PIN_RESET;
	for (int i = 0; i <= 8; i++) {
		GPIOA->BSRR = DIGIT_BCD_PIN_CLK;
		Delay_us(DIGIT_DELAY_US);
		GPIOA->BRR = DIGIT_BCD_PIN_CLK;
		Delay_us(DIGIT_DELAY_US);
	}
}

void DigitBCDEOL(int current_pos) {
	for (int i = current_pos; i <= 9; i++) {
		GPIOA->BSRR = DIGIT_BCD_PIN_CLK;
		Delay_us(DIGIT_DELAY_US);
		GPIOA->BRR = DIGIT_BCD_PIN_CLK;
		Delay_us(DIGIT_DELAY_US);
	}
}

void DigitBCDZero(void) {
	for (int i = 1; i <= 6; i++) {
			GPIOA->BSRR = DIGIT_BCD_PIN_CLK;
			Delay_us(DIGIT_DELAY_US);
			GPIOA->BRR = DIGIT_BYTE_CLEAN;
			Delay_us(DIGIT_DELAY_US);
			GPIOA->BRR = DIGIT_BCD_PIN_CLK;
			Delay_us(DIGIT_DELAY_US);
		}
}

void DigitBCDPrint(unsigned int num_high, unsigned int num_middle, unsigned int num_low) {
	uint8_t high_byte = INT_TO_BCD_8BIT(num_high);
	uint8_t mid_byte = INT_TO_BCD_8BIT(num_middle);
	uint8_t low_byte = INT_TO_BCD_8BIT(num_low);
	DigitBCDPrintByte(low_byte);
	DigitBCDPrintByte(mid_byte);
	DigitBCDPrintByte(high_byte);
	DigitBCDEOL(6);
}

void DigitBCDPrintByte(uint8_t bcd_byte) {
	GPIOA->BSRR = DIGIT_BCD_PIN_CLK;
	Delay_us(DIGIT_DELAY_US);
	GPIOA->BRR = DIGIT_BYTE_CLEAN;
	Delay_us(DIGIT_DELAY_US);
	GPIOA->BSRR = bcd_byte & 0x0F; //extract lower nibble by the 0x00001111 mask
	Delay_us(DIGIT_DELAY_US);
	GPIOA->BRR = DIGIT_BCD_PIN_CLK;
	Delay_us(DIGIT_DELAY_US);
	GPIOA->BSRR = DIGIT_BCD_PIN_CLK;
	Delay_us(DIGIT_DELAY_US);
	GPIOA->BRR = DIGIT_BYTE_CLEAN;
	Delay_us(DIGIT_DELAY_US);
	GPIOA->BSRR = (bcd_byte >> 4) & 0x0F; //extract higher nibble
	Delay_us(DIGIT_DELAY_US);
	GPIOA->BRR = DIGIT_BCD_PIN_CLK;
	Delay_us(DIGIT_DELAY_US);
}

void DigitBCDStartupRoll(void) {
	DigitBCDReset();
	GPIOA->BSRR = DIGIT_BCD_PIN_COLON;
	for (int i = 0; i <= 99; i++) {
		BuzzerUpdate();
		DigitBCDPrint(i, i, i);
		HAL_Delay(DIGIT_STARTUP_DELAY);
	}
}