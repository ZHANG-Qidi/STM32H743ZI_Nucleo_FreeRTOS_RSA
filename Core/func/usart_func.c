#include "usart_func.h"

#include "common_func.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

static uint32_t usart3_r_len;
static uint8_t usart3_r_message[UART_BUFFSIZE];
static DMA_BUFFER usart_str uart3_str;

void HAL_USART3_ENABLE_IT_IDLE_Receive_DMA() {
    __HAL_UART_CLEAR_IDLEFLAG(&huart3);
    __HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE);
    HAL_UART_Receive_DMA(&huart3, uart3_str.Uart_RecvBuff, UART_BUFFSIZE);
}

void HAL_USART3_IdleCallback() {
    if (__HAL_UART_GET_FLAG(&huart3, UART_FLAG_IDLE) != RESET && huart3.Instance == USART3) {
        __HAL_UART_CLEAR_IDLEFLAG(&huart3);
        uint32_t counter_0 = __HAL_DMA_GET_COUNTER(&hdma_usart3_rx);
        for (int32_t i = 1 << 16; i--;) {
        }
        uint32_t counter_1 = __HAL_DMA_GET_COUNTER(&hdma_usart3_rx);
        if (counter_0 == counter_1) {
            HAL_UART_DMAStop(&huart3);
            usart3_r_len = UART_BUFFSIZE - __HAL_DMA_GET_COUNTER(&hdma_usart3_rx);
            if (usart3_r_len < UART_BUFFSIZE - 1) {
                SCB_CleanInvalidateDCache_MACRO();
                memcpy(usart3_r_message, uart3_str.Uart_RecvBuff, usart3_r_len);
                usart3_r_message[usart3_r_len] = '\0';
            } else {
                usart3_r_len = 0;
            }
            HAL_UART_Receive_DMA(&huart3, uart3_str.Uart_RecvBuff, UART_BUFFSIZE);
        }
    }
}

int32_t usart3_printf(const char *format, ...) {
    while (__HAL_UART_GET_FLAG (&huart3, UART_FLAG_TC) == RESET) {
        for (int32_t i = 1 << 16; i--;) {
        }
    }
    va_list args;
    int32_t ret;
    va_start(args, format);
    ret = vsprintf((char*) uart3_str.Uart_SendBuff, format, args);
    if (ret > 0) {
        SCB_CleanInvalidateDCache_MACRO();
        HAL_UART_Transmit_DMA(&huart3, uart3_str.Uart_SendBuff, strlen((char*) uart3_str.Uart_SendBuff));
    }
    va_end(args);
    return ret;
}

int32_t usart3_scanf(const char *format, ...) {
    while (0 == usart3_r_len) {
        for (int32_t i = 1 << 16; i--;) {
        }
    }
    usart3_r_len = 0;
    va_list args;
    int32_t ret;
    va_start(args, format);
    ret = vsscanf((char*) usart3_r_message, format, args);
    va_end(args);
    return ret;
}
