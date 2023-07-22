#include "u6_transdata.h"

// void u6_dma_init(DMA_HandleTypeDef *hdma)
// {
//     hdma
// }

void trans_ccd_data(uint16_t *data)
{
    uint8_t buffer[2 * sizeof(data) + 2];
    uint8_t *p = buffer;
    *p++       = 0xff;
    for (uint16_t i = 1; i < sizeof(buffer); i++) {
        *p++ = (uint8_t)(data[i] >> 8);
        *p++ = (uint8_t)(data[i]);
    }
    *p = 0x00;
    HAL_UART_Transmit(&huart6, buffer, sizeof(buffer), 0xffff);
}