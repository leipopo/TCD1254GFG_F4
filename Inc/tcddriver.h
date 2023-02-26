#ifndef _TCD_DRIVER_H_
#define _TCD_DRIVER_H_

#include "tim.h"
#include "adc.h"
#define true              0x00
#define false             0x00
#define sclk              htim8
#define sclk_ch           TIM_CHANNEL_4
#define mastertick_period 500   // 500ns
#define t1                5000  // 5000ns
#define t2                500   // 500ns
#define t3                2000  // 1000ns
#define t4                20    // 20ns
#define tint_min          10000 // 10000ns

#define sh_iogroup        GPIOI
#define sh_io             GPIO_PIN_6
#define icg_iogroup       GPIOI
#define icg_io            GPIO_PIN_7
#define osadc             hadc1
#define osadc_ch          ADC_CHANNEL_8
#define shpullup          HAL_GPIO_WritePin(sh_iogroup, sh_io, GPIO_PIN_SET)
#define shpulldown        HAL_GPIO_WritePin(sh_iogroup, sh_io, GPIO_PIN_RESET)
#define icgpullup         HAL_GPIO_WritePin(icg_iogroup, icg_io, GPIO_PIN_SET)
#define icgpulldown       HAL_GPIO_WritePin(icg_iogroup, icg_io, GPIO_PIN_RESET)

typedef struct tcd_data {
    uint8_t switcher;
    uint16_t sh_tick;
    uint32_t master_tick;
    float voltage[2547];
} tcddata;

void scanstart(tcddata *t);
void TCD_RW(tcddata *t, float os_vvp);
void delay_ns(int16_t ns);
float init_vrefint_reciprocal(void);
static float get_os_signal(float voltage_vrefint_proportion);
static uint16_t adcx_get_chx_value(ADC_HandleTypeDef *ADCx, uint32_t ch);
#endif
