#ifndef _USART_GRUNT_
#define _USART_GRUNT_

#include "usart.h"

#define UART_BUFFSIZE 2048   // 定义缓冲区的大小

typedef struct {
    uint8_t Uart_SendBuff[UART_BUFFSIZE];
    uint8_t Uart_RecvBuff[UART_BUFFSIZE];
} usart_str;

// include
extern UART_HandleTypeDef huart3;
extern DMA_HandleTypeDef hdma_usart3_rx;
extern DMA_HandleTypeDef hdma_usart3_tx;

void HAL_USART3_ENABLE_IT_IDLE_Receive_DMA();
void HAL_USART3_IdleCallback();
int32_t usart3_printf(const char *format, ...);
int32_t usart3_scanf(const char *format, ...);

#endif
