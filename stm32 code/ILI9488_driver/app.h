#ifndef MAIN_H_
#define MAIN_H_

#define u8 			uint8_t
#define u16 		uint16_t
#define u32 		uint32_t
#define u64			uint64_t

#define LOG(str)	uart1_printSTR(str)
#define LOGn(num)   uart1_printNUM(num)
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "stm32f0xx_hal.h"
#include "fatfs.h"

//#include "ili9488.hpp"
extern ADC_HandleTypeDef hadc;
extern I2C_HandleTypeDef hi2c2;
extern SPI_HandleTypeDef hspi1;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;



void app_init(void);
void app_loop(void);

void uart1_init(void);
void uart1_tx(u8 *pdata, u8 len);
void uart1_rx(u8 *pdata, u8 len);
void uart1_printSTR(const char buf[]);
void uart1_printNum(int64_t num);

void file_init(void);
void file_download(const char filename[]);
void file_upload(const char filename[],u32 filesize,u16 packagesize);


#endif /* MAIN_HPP_ */
