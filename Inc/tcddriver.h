#ifndef _TCD_DRIVER_H_
#define _TCD_DRIVER_H_

#include "tim.h"
#include "adc.h"
#include "dma.h"
#include "math.h"
#include "stm32f4xx_hal_dma.h"

#include "u6_transdata.h"

#define ccdpixel_width                  0.00525f // ccd像素宽度 毫米
#define focal_length                    15.18f   // 聚光凸透镜焦距
#define install_distance                11.f     // 光源轴心到透镜主光轴的安装距离
#define pixeltodistance(pixel_position) focal_length + focal_length *install_distance / ((2500 - pixel_position) * ccdpixel_width)

#define mastertick_period               500   // 500ns
#define t1                              5000  // 5000ns
#define t2                              500   // 500ns
#define t3                              1000  // 1000ns
#define t4                              20    // 20ns
#define tint_min                        10000 // 10000ns

#define ms_tim                          htim8
#define ms_tim_ch                       TIM_CHANNEL_4

#define sh_tim                          htim2
#define sh_tim_ch                       TIM_CHANNEL_1

#define icg_tim                         htim4
#define icg_tim_ch                      TIM_CHANNEL_1

#define os_tim                          htim3
#define os_tim_ch                       TIM_CHANNEL_1

#define osadc                           hadc1
#define osadc_ch                        ADC_CHANNEL_8

#define ccdsize                         3000

#define sampletime                      50 // 采样次数
#define emptysampletime                 4  // 空采样次数

typedef struct
{
    int16_t streamcount;
    uint16_t sumdata[ccdsize];
} CCDDATASOLVER;

void scanstart(CCDDATASOLVER *sol);
void scanstop(CCDDATASOLVER *sol);
void tcdinit(struct __DMA_HandleTypeDef *hdma);
extern volatile uint16_t CCDDataBuffer[ccdsize];
extern DMA_HandleTypeDef hdma_adc1;
#endif
