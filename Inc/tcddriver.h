#ifndef _TCD_DRIVER_H_
#define _TCD_DRIVER_H_

#include "tim.h"
#include "adc.h"
#include "dma.h"
#include "math.h"
#include "stm32f4xx_hal_dma.h"
#include "u6_transdata.h"
#include "KF.h"
#include "FB_filter.h"

#define ccdpixel_width              0.00525f // ccd像素宽度 毫米
#define focal_length                15.18f   // 聚光凸透镜焦距
#define install_distance            11.f     // 光源轴心到透镜主光轴的安装距离

#define mastertick_period           500   // 500ns
#define t1                          5000  // 5000ns
#define t2                          500   // 500ns
#define t3                          1000  // 1000ns
#define t4                          20    // 20ns
#define tint_min                    10000 // 10000ns

#define ms_tim                      htim8
#define ms_tim_ch                   TIM_CHANNEL_4

#define sh_tim                      htim2
#define sh_tim_ch                   TIM_CHANNEL_1

#define icg_tim                     htim4
#define icg_tim_ch                  TIM_CHANNEL_1

#define os_tim                      htim3
#define os_tim_ch                   TIM_CHANNEL_1

#define osadc                       hadc1
#define osadc_ch                    ADC_CHANNEL_8

#define ccdsize                     2800

#define sampletime                  5 // 采样次数
#define emptysampletime             4 // 空采样次数

#define lowestdiif0pointx_samplenum 500

#define iirorder                    3
typedef struct
{
    int16_t streamcount;
    float sumdata[ccdsize];
    float iir_data[ccdsize];
    float diff_data[ccdsize - 1];
    uint16_t diff_zeropointsx[ccdsize - 1];
    uint16_t lowerdiff_zeropointsx[ccdsize - 1];
} CCDDATASOLVER;

void scanstart(CCDDATASOLVER *sol);
void scanstop(CCDDATASOLVER *sol, FBFILTER *iirf);
void tcdinit(struct __DMA_HandleTypeDef *hdma, FBFILTER *iirf, KF_1thorder *kf);
// void filtfilt(IIRFILTER *iirf, uint16_t *input, uint16_t len, float *output);
void diff_calc(float *data, uint16_t len, float *diff_data);
void find_0diffpoints(float *diffdata, uint16_t len, uint16_t *diff_0pointsx);
void find_lower0diffpoinsts(float *data, uint16_t threshold, uint16_t *diff_0pointsx, uint16_t len, uint16_t *lowerdiff_0pointsx);
extern volatile uint16_t CCDDataBuffer[ccdsize];
extern CCDDATASOLVER cds;
extern FBFILTER iirfilter;
extern KF_1thorder kf;
extern DMA_HandleTypeDef hdma_adc1;
#endif
