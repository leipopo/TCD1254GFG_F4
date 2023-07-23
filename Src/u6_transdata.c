#include "u6_transdata.h"

// void u6_dma_init(DMA_HandleTypeDef *hdma)
// {
//     hdma
// }

void trans_ccd_data(uint16_t *data, uint16_t size)
{
    uint8_t Senbuff[size * 2 + 2];
    Senbuff[size * 2]     = 0x0A;
    Senbuff[size * 2 + 1] = 0x0D;
    for (uint16_t i = 0; i < size; i++) {
        Senbuff[i * 2 + 1] = data[i] & 0xFF;
        Senbuff[i * 2 + 2] = (data[i] >> 8) & 0xFF;
    }
    HAL_UART_Transmit_DMA(&huart6, (uint8_t *)Senbuff, sizeof(Senbuff));
    while (HAL_UART_GetState(&huart6) != HAL_UART_STATE_READY)
        ;
}