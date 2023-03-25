#ifndef _COMMON_FUNC_H_
#define _COMMON_FUNC_H_

#include "stm32h743xx.h"

#define SECTION_ITCM __attribute__((section(".itcm")))
#define SECTION_DTCM __attribute__ ((section (".dtcm")))
#define DMA_BUFFER __attribute__((section(".dma_buffer")))
#define BDMA_BUFFER __attribute__((section(".bdma_buffer")))

#define SCB_CleanInvalidateDCache_MACRO() SCB_CleanInvalidateDCache()
#define SECTION_DATA SECTION_DTCM
#define SECTION_FLASH SECTION_ITCM

#define LED1_Pin GPIO_PIN_0
#define LED1_GPIO_Port GPIOB

#define LED2_Pin GPIO_PIN_7
#define LED2_GPIO_Port GPIOB

#define LED3_Pin GPIO_PIN_14
#define LED3_GPIO_Port GPIOB

#endif
