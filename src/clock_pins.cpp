/*
*   @file   clock_pins.cpp
*   @brief  Pins definitions and inits file
*/

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"
#include "clock_pins.h"

void DigitPins_Init(void);
void RTCPins_Init(void);

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

void RTCPins_Init(void) {
	__HAL_RCC_GPIOB_CLK_ENABLE();
}

void BuzzerPins_Init(void) {
	__HAL_RCC_GPIOA_CLK_ENABLE();
	
	// Настройка пина PA8 как альтернативной функции (TIM1_CH1)
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}