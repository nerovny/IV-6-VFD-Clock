// Buzzer handling defs

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_tim.h"
#include <stdbool.h>

typedef enum {
	BUZZER_STATE_IDLE		= 0x00U,
	BUZZER_STATE_ACTIVE		= 0x01U
} BUZZER_StateTypeDef;

void BuzzerInit(void);
void BuzzerStart(uint32_t frequency, uint32_t duty);
void BuzzerStartDuration(uint32_t frequency, uint32_t duty, uint32_t duration);
void BuzzerStop(void);
void BuzzerUpdate(void);

bool isBuzzerActive(void);

void MX_TIM1_Init(void);