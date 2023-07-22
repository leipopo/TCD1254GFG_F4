#include "tcddriver.h"

volatile uint16_t CCDDataBuffer[ccdsize];

void dmatransfercomplete(struct __DMA_HandleTypeDef *hdma)
{
    HAL_ADC_Stop_DMA(&hadc1);
}

void tcdinit(struct __DMA_HandleTypeDef *hdma)
{
    HAL_TIM_Base_Start(&ms_tim);
    HAL_TIM_PWM_Start(&ms_tim, ms_tim_ch);
    HAL_TIM_Base_Start(&icg_tim);
    HAL_TIM_OnePulse_Start(&icg_tim, icg_tim_ch);
    HAL_TIM_Base_Start(&sh_tim);
    HAL_TIM_OnePulse_Start(&sh_tim, sh_tim_ch);
    HAL_TIM_Base_Start(&os_tim);
    HAL_TIM_PWM_Start(&os_tim, os_tim_ch);
    hdma->XferCpltCallback = dmatransfercomplete;
}

void scanstart(CCDDATASOLVER *sol)
{
    if (sol->streamcount == 0) {
        __HAL_TIM_ENABLE(&ms_tim);
        __HAL_TIM_ENABLE(&icg_tim);
        __HAL_TIM_ENABLE(&sh_tim);
        __HAL_TIM_ENABLE(&os_tim);
    }

    else if (sol->streamcount >= 1 && sol->streamcount <= sampletime) {
        HAL_ADC_Start_DMA(&hadc1, (uint32_t *)CCDDataBuffer, ccdsize);
        __HAL_TIM_ENABLE(&icg_tim);
        __HAL_TIM_ENABLE(&sh_tim);
    }
}

void scanstop(CCDDATASOLVER *sol)
{
    HAL_Delay(2);
    if (sol->streamcount == 1) {
        for (uint16_t i = 0; i < ccdsize; i++) {
            sol->sumdata[i] = CCDDataBuffer[i];
        }
    } else if (sol->streamcount <= sampletime) {
        for (uint16_t i = 0; i < ccdsize; i++) {
            sol->sumdata[i] += CCDDataBuffer[i];
        }
    }

    if (sol->streamcount == sampletime) {
        __HAL_TIM_DISABLE(&icg_tim);
        __HAL_TIM_DISABLE(&sh_tim);
        __HAL_TIM_DISABLE(&os_tim);
        __HAL_TIM_DISABLE(&ms_tim);
        sol->streamcount = 0;
        trans_ccd_data((uint16_t *)sol->sumdata);
    } else {
        sol->streamcount++;
    }
    // trans_ccd_data((uint16_t *)CCDDataBuffer);
}
