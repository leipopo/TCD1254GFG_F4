#include "tcddriver.h"
#include "math.h"

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
    steppos((uint16_t *)CCDDataBuffer, ccdsize, cds);
}

// 此函数假定adc采样中除离开Dummy区外有一个下降沿
void steppos(uint16_t *data, uint32_t size, CCDDATASOLVER *cds)
{

    for (uint32_t i = dummyposition; i < size; i++) // 记录此次采样的位置
    {
        if (data[i] < stepsthreshold) {
            cds->postion[cds->streamcount] = i;
            cds->sum += i;
		
        cds->streamcount++;
            break;
        } 
    }

    if (cds->streamcount == sampletime ) {

        cds->result[0] = cds->sum / sampletime;
        for (uint16_t i = 0; i < sampletime; i++) {
            if (fabs((double)cds->postion[i] - (double)cds->result[0]) > 5) {
                cds->streamcount--;
                cds->sum -= cds->postion[i];
            }
            cds->postion[i]=0;
        }
        cds->result[0]   = cds->sum / (cds->streamcount);
        cds->result[1]   = pixeltodistance(cds->result[0]);
        cds->streamcount = 0;
        cds->sum         = 0;

    } 
}
