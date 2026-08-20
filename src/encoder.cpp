#include "encoder.h"
#include "stm32f1xx_hal_gpio.h"

void Encoder_Init(Encoder_t* data, GPIO_TypeDef* GPIO_A_Port, uint16_t GPIO_A_Pin, GPIO_TypeDef* GPIO_B_Port, uint16_t GPIO_B_Pin) {
    data->GPIO_A = GPIO_A_Port;
    data->GPIO_B = GPIO_B_Port;
    data->GPIO_PIN_A = GPIO_A_Pin;
    data->GPIO_PIN_B = GPIO_B_Pin;

    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_AFIO_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = data->GPIO_PIN_A;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(data->GPIO_A, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = data->GPIO_PIN_B;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(data->GPIO_B, &GPIO_InitStruct);

    // Enable and set EXTI15_10 Interrupt in the NVIC
    HAL_NVIC_SetPriority(EXTI15_10_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

    /* Set default mode */
    data->Mode = Encoder_Mode_Normal;

    /* Set defaults */
    data->RE_Count = 0;
}

Encoder_Rotation_t Encoder_Get(Encoder_t* data) {
    if (data->RE_Count < 0) {
        data->Rotation = Encoder_Rotate_Decrement;
        data->RE_Count = 0;
        return Encoder_Rotate_Decrement;
    } else if (data->RE_Count > 0) {
        data->Rotation = Encoder_Rotate_Increment;
        data->RE_Count = 0;
        return Encoder_Rotate_Increment;
    }

    data->Rotation = Encoder_Rotate_Nothing;
    data->RE_Count = 0;
    return Encoder_Rotate_Nothing;
}

void Encoder_SetMode(Encoder_t* data, Encoder_Mode_t mode) {
    data->Mode = mode;
}

void Encoder_Process(Encoder_t* data) {
    uint8_t now_b;
    now_b = HAL_GPIO_ReadPin(data->GPIO_B, data->GPIO_PIN_B);

    if (now_b == 1) data->RE_Count++;
    else data->RE_Count--;
}