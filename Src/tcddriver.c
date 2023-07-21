#include "tcddriver.h"

extern DMA_HandleTypeDef hdma_adc1;

volatile uint16_t CCDDataBuffer[ccdsize];

void dmatransfercomplete(struct __DMA_HandleTypeDef *hdma)
{
    HAL_ADC_Stop_DMA(&hadc1);
}

void tcdinit(void)
{
    HAL_TIM_Base_Start(&ms_tim);
    HAL_TIM_PWM_Start(&ms_tim, ms_tim_ch);
    HAL_TIM_Base_Start(&icg_tim);
    HAL_TIM_OnePulse_Start(&icg_tim, icg_tim_ch);
    HAL_TIM_Base_Start(&sh_tim);
    HAL_TIM_OnePulse_Start(&sh_tim, sh_tim_ch);
    HAL_TIM_Base_Start(&os_tim);
    HAL_TIM_PWM_Start(&os_tim, os_tim_ch);
    hdma_adc1.XferCpltCallback = dmatransfercomplete;
}

void scanstart(void)
{
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)CCDDataBuffer, ccdsize);
    __HAL_TIM_ENABLE(&icg_tim);
    __HAL_TIM_ENABLE(&sh_tim);
}

void scanstop(CCDDATASOLVER *cds)
{
    HAL_Delay(2);
    steppos((uint16_t *)CCDDataBuffer, ccdsize, cds);
}

// 此函数假定adc采样中除离开Dummy区外有一个下降沿
void steppos(uint16_t *data, uint32_t size, CCDDATASOLVER *cds)
{

    // for (uint32_t i = dummyposition; i < size; i++) // 记录此次采样的位置
    // {
    //     if (data[i] < stepsthreshold) {
    //         cds->postion[cds->streamcount] = i;
    //         cds->sum += i;

    //         cds->streamcount++;
    //         break;
    //     }
    // }

    // if (cds->streamcount == sampletime) {

    //     cds->result[0] = cds->sum / sampletime;
    //     for (uint16_t i = 0; i < sampletime; i++) {
    //         if (fabs((double)cds->postion[i] - (double)cds->result[0]) > 5) {
    //             cds->streamcount--;
    //             cds->sum -= cds->postion[i];
    //         }
    //         cds->postion[i] = 0;
    //     }
    //     cds->result[0]   = cds->sum / (cds->streamcount);
    //     cds->result[1]   = pixeltodistance(cds->result[0]);
    //     cds->streamcount = 0;
    //     cds->sum         = 0;
    // }
}
