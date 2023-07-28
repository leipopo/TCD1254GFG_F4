#include "u6_transdata.h"

void trans_ccd_data_uint16(uint16_t *data, uint16_t size, uint8_t headflag)
{
    uint8_t Senbuff[size * 2 + 3];
    Senbuff[0]            = headflag;
    Senbuff[size * 2 + 1] = 0x0A;
    Senbuff[size * 2 + 2] = 0x0D;
    for (uint16_t i = 0; i < size; i++) {
        Senbuff[i * 2 + 1] = data[i] & 0xFF;
        Senbuff[i * 2 + 2] = (data[i] >> 8) & 0xFF;
    }
    HAL_UART_Transmit_DMA(&huart6, (uint8_t *)Senbuff, sizeof(Senbuff));
    while (HAL_UART_GetState(&huart6) != HAL_UART_STATE_READY)
        ;
}

void trans_ccd_data_float(float *data, uint16_t size, uint8_t headflag)
{
    uint8_t Senbuff[size * 2 + 3];
    Senbuff[0]            = headflag;
    Senbuff[size * 2 + 1] = 0x0A;
    Senbuff[size * 2 + 2] = 0x0D;
    for (uint16_t i = 0; i < size; i++) {
        Senbuff[i * 2 + 1] = ((uint16_t)data[i]) & 0xFF;
        Senbuff[i * 2 + 2] = (((uint16_t)data[i]) >> 8) & 0xFF;
    }
    HAL_UART_Transmit_DMA(&huart6, (uint8_t *)Senbuff, sizeof(Senbuff));
    while (HAL_UART_GetState(&huart6) != HAL_UART_STATE_READY)
        ;
}
