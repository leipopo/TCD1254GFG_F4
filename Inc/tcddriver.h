#ifndef _TCD_DRIVER_H_
#define _TCD_DRIVER_H_

#include "tim.h"
#include "adc.h"

#define mastertick_period 500   // 500ns
#define t1                5000  // 5000ns
#define t2                500   // 500ns
#define t3                1000  // 1000ns
#define t4                20    // 20ns
#define tint_min          10000 // 10000ns

#define ms_tim htim8
#define ms_tim_ch TIM_CHANNEL_4

#define sh_tim htim5
#define sh_tim_ch TIM_CHANNEL_4

#define icg_tim htim4
#define icg_tim_ch TIM_CHANNEL_1

#define os_tim htim3
#define os_tim_ch TIM_CHANNEL_1

#define osadc             hadc1
#define osadc_ch          ADC_CHANNEL_8

#define ccdsize 3000


void scanstart();
void delay_ns(int16_t ns);
float init_vrefint_reciprocal(void);
#endif
