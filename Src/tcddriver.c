#include "tcddriver.h"

volatile uint16_t CCDDataBuffer[ccdsize];
void scanstart(void)
{
    HAL_TIM_PWM_Start(&os_tim, os_tim_ch);
    __HAL_TIM_ENABLE(&icg_tim);
    __HAL_TIM_ENABLE(&sh_tim);
	HAL_ADC_Start_DMA(&hadc1, (uint32_t *)CCDDataBuffer, ccdsize);
}

int scanstop(void)
{
    HAL_Delay(2);
    HAL_ADC_Stop_DMA(&hadc1);
    HAL_TIM_PWM_Stop(&os_tim, os_tim_ch);
    return stepsdistance((uint16_t *)CCDDataBuffer, ccdsize);
}

// 此函数假定adc采样中有两个阶跃
int stepsdistance(uint16_t *data, uint32_t size)
{
    uint32_t steppos[2] = {0, 0};
    for (uint32_t i = 0; i < size; i++) {
//        if ((data[i + 1] - data[i] > stepsthreshold_d)&&((data[i+2]+data[i+1])/2>data[i+1])&&((data[i+2]-data[i+1])<interftol_d)) {
					if ( data[i+1] > 2300&&data[i] <2300&&((data[i+2]-data[i+1])<interftol_d)) {
            if (steppos[0] == 0) {
                steppos[0] = i;
            } else if (steppos[1] == 0) {
							if( i - steppos[0]<200)
                return i - steppos[0];
            }
        }
    }
   return 0;
}
