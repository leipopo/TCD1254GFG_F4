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

#define sh_tim htim2
#define sh_tim_ch TIM_CHANNEL_1

#define icg_tim htim4
#define icg_tim_ch TIM_CHANNEL_1

#define os_tim htim3
#define os_tim_ch TIM_CHANNEL_1

#define osadc             hadc1
#define osadc_ch          ADC_CHANNEL_8

#define ccdsize 2600

#define stepsthreshold_d 1000
#define interftol_d 1000 

#define sampletime 20 
typedef struct CCDDATASOLVER
{
    uint8_t streamstate;
    uint8_t streamcount;
    uint16_t sampledata[sampletime][ccdsize];
    uint32_t postion[sampletime]; 
};


void scanstart(void);
int stepsdistance(uint16_t *data, uint32_t size);
void scanstop(void);
extern volatile uint16_t CCDDataBuffer[ccdsize];
#endif
