/*
*   @file   clock_pins.h
*   @brief  Pins definitions and inits header file
*/

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

void DigitPins_Init(void);