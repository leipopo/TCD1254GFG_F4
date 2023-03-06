#include "tcddriver.h"

volatile uint16_t CCDDataBuffer[ccdsize];
void scanstart(void)
{
    //HAL_TIM_PWM_Start(&os_tim, os_tim_ch);
	
		HAL_ADC_Start_DMA(&hadc1, (uint32_t *)CCDDataBuffer, ccdsize);
    __HAL_TIM_ENABLE(&icg_tim);
    __HAL_TIM_ENABLE(&sh_tim);
}

void scanstop(void)
{
    HAL_Delay(2);
    HAL_ADC_Stop_DMA(&hadc1);
    //HAL_TIM_PWM_Stop(&os_tim, os_tim_ch);
    //return stepsdistance((uint16_t *)CCDDataBuffer, ccdsize);
}

// 此函数假定adc采样中有一个阶跃
// int stepsdistance(uint16_t *data, uint32_t size)
// {
    
// }
