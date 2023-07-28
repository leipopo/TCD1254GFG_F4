#include "tcddriver.h"

volatile uint16_t CCDDataBuffer[ccdsize];

FBFILTER iirfilter;
CCDDATASOLVER cds;
KF_1thorder kf;

uint16_t lowestdiif0pointx[lowestdiif0pointx_samplenum] = {0};
uint16_t lowestdiif0pointx_KF                           = 0;
uint16_t last_low_diif0pointx                           = 0;

float b[3] = {2.4619300464140628e-06, 4.9238600928281255e-06, 2.4619300464140628e-06};
float a[3] = {1.0, -1.995557124345789, 0.9955669720659747};

void dmatransfercomplete(struct __DMA_HandleTypeDef *hdma)
{
    HAL_ADC_Stop_DMA(&hadc1);
}

void tcdinit(struct __DMA_HandleTypeDef *hdma, FBFILTER *iirf, KF_1thorder *kf)
{
    HAL_TIM_Base_Start(&ms_tim);
    HAL_TIM_PWM_Start(&ms_tim, ms_tim_ch);
    HAL_TIM_Base_Start(&icg_tim);
    HAL_TIM_OnePulse_Start(&icg_tim, icg_tim_ch);
    HAL_TIM_Base_Start(&sh_tim);
    HAL_TIM_OnePulse_Start(&sh_tim, sh_tim_ch);
    HAL_TIM_Base_Start(&os_tim);
    HAL_TIM_PWM_Start(&os_tim, os_tim_ch);

    hdma->XferCpltCallback = dmatransfercomplete; // 传输完成回调函数

    fbfilter_init(iirf, b, a, iirorder); // 初始化二阶IIR滤波器

    kalmanfilter_1th_INIT(kf, 0.01, 4, 0.1, 1, 1);
}

void scanstart(CCDDATASOLVER *sol)
{
    if (sol->streamcount == 0) {
        __HAL_TIM_ENABLE(&ms_tim);
        __HAL_TIM_ENABLE(&icg_tim);
        __HAL_TIM_ENABLE(&sh_tim);
        __HAL_TIM_ENABLE(&os_tim);
    }

    else if (sol->streamcount >= emptysampletime && sol->streamcount <= sampletime + emptysampletime - 1) {
        HAL_ADC_Start_DMA(&hadc1, (uint32_t *)CCDDataBuffer, ccdsize);
        __HAL_TIM_ENABLE(&icg_tim);
        __HAL_TIM_ENABLE(&sh_tim);
    } else {
        __HAL_TIM_ENABLE(&icg_tim);
        __HAL_TIM_ENABLE(&sh_tim);
    }
}

uint16_t find_last_low_diff0_point(float *data, uint16_t threshold, uint16_t *diff_0pointsx, uint16_t len)
{
    uint16_t temp = 0;
    for (uint16_t i = len - 500; i >= 0; i--) {
        if (data[diff_0pointsx[i]] < threshold) {
            temp = diff_0pointsx[i];
            break;
        }
    }
    return temp;
}

uint16_t find_lowest_diff0_point(float *data, uint16_t *diff_0pointsx, uint16_t len)
{
    uint16_t temp = data[1];
    for (uint16_t i = 0; i < len; i++) {
        if (data[diff_0pointsx[i]] < temp) {
            temp = diff_0pointsx[i];
            break;
        }
    }
    for (uint16_t i = 0; i < len; i++) {
        diff_0pointsx[i] = 0;
    }
    return temp;
}

void scanstop(CCDDATASOLVER *sol, FBFILTER *iirf)
{
    HAL_Delay(2);
    // filtfilt(iirf, (uint16_t *)CCDDataBuffer, ccdsize, sol->iir_data);
    if (sol->streamcount == emptysampletime) {
        for (uint16_t i = 0; i < ccdsize; i++) {
            sol->sumdata[i] = CCDDataBuffer[i];
        }
    } else if (sol->streamcount < sampletime + emptysampletime && sol->streamcount > emptysampletime) {
        for (uint16_t i = 0; i < ccdsize; i++) {
            sol->sumdata[i] += CCDDataBuffer[i];
        }
    }

    if (sol->streamcount == sampletime + emptysampletime - 1) {
        __HAL_TIM_DISABLE(&icg_tim);
        __HAL_TIM_DISABLE(&sh_tim);
        __HAL_TIM_DISABLE(&os_tim);
        __HAL_TIM_DISABLE(&ms_tim);
        sol->streamcount = 0;
        fbfilter(iirf, sol->sumdata, sol->iir_data, ccdsize);
        diff_calc(sol->iir_data, ccdsize, sol->diff_data);
        find_0diffpoints(sol->diff_data, ccdsize, sol->diff_zeropointsx);
        // find_lower0diffpoinsts(sol->iir_data, 1500, sol->diff_zeropointsx, ccdsize, sol->lowerdiff_zeropointsx);
        // last_low_diif0pointx = find_last_low_diff0_point(sol->iir_data, 1400, sol->diff_zeropointsx, ccdsize);
        // for (uint16_t i = 0; i < lowestdiif0pointx_samplenum - 1; i++) {
        //     lowestdiif0pointx[i] = lowestdiif0pointx[i + 1];
        // }
        lowestdiif0pointx[lowestdiif0pointx_samplenum - 1] = find_lowest_diff0_point(sol->iir_data, sol->diff_zeropointsx, ccdsize);
        lowestdiif0pointx_KF                               = kalmanfilter_1th(&kf, lowestdiif0pointx[lowestdiif0pointx_samplenum - 1]);
        // if (lowestdiif0pointx[0] != 0) {
        //     trans_ccd_data_float(sol->iir_data, ccdsize, 0xff);
        //     trans_ccd_data_float(sol->sumdata, ccdsize, 0x00);
        //     trans_ccd_data_uint16(lowestdiif0pointx, lowestdiif0pointx_samplenum, 0x0f);
        // }
        
    } else {
        sol->streamcount++;
    }
}

void diff_calc(float *data, uint16_t len, float *diff)
{
    for (uint16_t i = 0; i < len - 1; i++) {
        diff[i] = data[i + 1] - data[i];
    }
}

void find_0diffpoints(float *diffdata, uint16_t len, uint16_t *diff_0pointsx)
{
    uint16_t temp_count = 0;
    for (uint16_t i = 0; i < len - 1; i++) {
        if (diffdata[i] * diffdata[i + 1] <= 0) {
            diff_0pointsx[temp_count] = i;
            temp_count++;
        }
    }
}

void find_lower0diffpoinsts(float *data, uint16_t threshold, uint16_t *diff_0pointsx, uint16_t len, uint16_t *lowerdiff_0pointsx)
{
    uint16_t temp_count = 0;
    for (uint16_t i = 0; i < len - 1; i++) {
        if (data[i] < threshold) {
            lowerdiff_0pointsx[temp_count] = i;
            temp_count++;
        }
    }
}
