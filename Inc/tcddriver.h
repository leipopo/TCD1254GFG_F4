#ifndef _TCD_DRIVER_H_
#define _TCD_DRIVER_H_

#include "tim.h"
#include "adc.h"
#define true              0x01
#define false             0x00
#define sclk              htim3
#define mastertick_period 500   // 500ns
#define t1                5000  // 5000ns
#define t2                500   // 500ns
#define t3                2000  // 1000ns
#define t4                20    // 20ns
#define tint_min          10000 // 10000ns

#define sh_iogroup        GPIOA
#define sh_io             GPIO_PIN_5
#define icg_iogroup       GPIOA
#define icg_io            GPIO_PIN_7
#define osadc            hadc1
#define osadc_ch          ADC_CHANNEL_8

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
