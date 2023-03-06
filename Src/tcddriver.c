#include "tcddriver.h"

volatile uint16_t CCDDataBuffer[ccdsize];
void scanstart(void)
{
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)CCDDataBuffer, ccdsize);
    __HAL_TIM_ENABLE(&icg_tim);
    __HAL_TIM_ENABLE(&sh_tim);
}

void scanstop(CCDDATASOLVER *cds)
{
    HAL_Delay(2);
    HAL_ADC_Stop_DMA(&hadc1);
    steppos((uint16_t *)CCDDataBuffer, ccdsize,cds);
}

// 此函数假定adc采样中有一个阶跃
void steppos(uint16_t *data, uint32_t size, CCDDATASOLVER *cds)
{
    if (cds->streamstate == 1) {
        for(uint32_t i = 0; i < size; i++) {
            if (data[i]<stepsthreshold) {
                cds->postion[cds->streamcount] = i;
                break;
            }
        }

        if (cds->streamcount == sampletime - 1) {
            cds->streamcount = 0;
            cds->streamstate = 0;
            for(uint32_t i = 0; i < sampletime; i++) {
                cds->result += cds->postion[i];
            }
            cds->result /= sampletime;
        } else {
            cds->streamcount++;
        }
    }
}
