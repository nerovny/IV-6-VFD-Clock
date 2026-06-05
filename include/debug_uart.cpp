#include "stm32f103xb.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"
#include "debug_uart.h"


static void MX_USART1_UART_Init(void)
{
  huart1.Instance = USART1;
  huart1.Init.BaudRate = USART1_BAUDRATE;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
}

// Конфигурация пинов для USART1 (PA9 - TX, PA10 - RX)
static void MX_USART1_GPIO_Init(void)
{
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_USART1_CLK_ENABLE();
  
  GPIO_InitTypeDef GPIO_InitStructTX = {0};
  GPIO_InitStructTX.Pin = USART1_PIN_TX;
  GPIO_InitStructTX.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStructTX.Pull = GPIO_NOPULL;
  GPIO_InitStructTX.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructTX);

  GPIO_InitTypeDef GPIO_InitStructRX = {0};
  GPIO_InitStructRX.Pin = USART1_PIN_RX;
  GPIO_InitStructRX.Mode = GPIO_MODE_AF_INPUT;
  GPIO_InitStructRX.Pull = GPIO_PULLUP;
  GPIO_InitStructRX.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructRX);
}