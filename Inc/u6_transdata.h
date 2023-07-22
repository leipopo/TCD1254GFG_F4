#ifndef _6_TRANSDATA_H_
#define _6_TRANSDATA_H_

#include <stdio.h>
#include <string.h>
#include "usart.h"

void trans_ccd_data(uint16_t *data);
extern DMA_HandleTypeDef hdma_usart6_tx;
#endif
