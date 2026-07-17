// Buzzer handling defs

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_tim.h"
#include "buzzer.h"

TIM_HandleTypeDef htim1;
BUZZER_StateTypeDef buzstate;

volatile uint32_t buzzer_ticks = 0;
volatile uint32_t buzzer_start_time = 0;

void MX_TIM1_Init(void) {
	// Запуск ШИМ: HAL_TIM_PWM_Start(TIM_HandleTypeDef *htim, uint32_t Channel) 
	// Остановка ШИМ: HAL_TIM_PWM_Stop(TIM_HandleTypeDef *htim, uint32_t Channel)
	// Изменение частоты: __HAL_TIM_SET_AUTORELOAD(&htim1, period);
	// Изменение скважности: __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, i);

  	// Настройка таймера. Частота = 72 МГц
	__HAL_RCC_TIM1_CLK_ENABLE();
	htim1.Instance = TIM1;
	htim1.Init.Prescaler = 0 ;       // Таймер на частоте шины
  	htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  	htim1.Init.Period = 1000 - 1;
  	htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  	HAL_TIM_PWM_Init(&htim1);

    // Настройка канала ШИМ
    TIM_OC_InitTypeDef sConfigOC = {0};
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 500;              // Ширина импульса (скважность 50%)
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1);
}

void BuzzerInit(void) {
	MX_TIM1_Init();
	buzstate = BUZZER_STATE_IDLE;
}

// Бесконечный старт пищалки с частотой frequency (Hz) и скважностью duty (1..100%)
void BuzzerStart(uint32_t frequency, uint32_t duty) {
	uint32_t period = (1000000/frequency) - 1;
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	__HAL_TIM_SET_AUTORELOAD(&htim1, period);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, period*duty*0.01);
	buzstate = BUZZER_STATE_ACTIVE;
}

// Старт пищалки с частотой frequency (Hz), скважностью duty (1..100%) и длительностью duration (ms)
void BuzzerStartDuration(uint32_t frequency, uint32_t duty, uint32_t duration) {
	BuzzerStart(frequency, duty);
	buzzer_ticks = duration;
	buzzer_start_time = HAL_GetTick();
}

void BuzzerStop(void) {
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
	buzstate = BUZZER_STATE_IDLE;
}

bool isBuzzerActive(void) {
	if (buzstate == BUZZER_STATE_ACTIVE) {
		return true;
	} else return false;
}

void BuzzerUpdate(void) {
	if (isBuzzerActive()) {
			if (HAL_GetTick() - buzzer_start_time >= buzzer_ticks) {
				BuzzerStop();
			}
	}
}