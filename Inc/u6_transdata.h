#ifndef _6_TRANSDATA_H_
#define _6_TRANSDATA_H_

#include <stdio.h>
#include <string.h>
#include "usart.h"

void trans_ccd_data_uint16(uint16_t *data, uint16_t size, uint8_t headflag);
void trans_ccd_data_float(float *data, uint16_t size, uint8_t headflag);
extern DMA_HandleTypeDef hdma_usart6_tx;
#endif
