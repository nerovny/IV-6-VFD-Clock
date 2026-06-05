
/*
 * UART1 для отладочных целей
 *
 * Применение:
 * Отправка данных (массив, размер, таймаут)
 *      HAL_UART_Transmit(&huart1, uint8_t tx_data, sizeof(tx_data) - 1, 1000); 
 * Прием данных (буфер, размер, таймаут)
 *      HAL_UART_Receive(&huart1, uint8_t rx_buffer, 5, 500);
 */

#include "stm32f103xb.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"

UART_HandleTypeDef huart1;

#define USART1_BAUDRATE     9600;
#define USART1_PIN_TX       GPIO_PIN_9;
#define USART1_PIN_RX       GPIO_PIN_10;

static void MX_USART1_UART_Init(void);
static void MX_USART1_GPIO_Init(void);