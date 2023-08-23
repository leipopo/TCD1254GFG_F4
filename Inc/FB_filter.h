#ifndef __FB_FILTER_H
#define __FB_FILTER_H
#include <stdlib.h>
#include "stdint.h"
#include "math.h"

typedef struct __FBFILTER {
    float *b;
    float *a;
    float *x;
    float *y;
    float *zi;
    uint8_t order;

    void (*zi_calc)(struct __FBFILTER *fb);
    void (*initvalue_calc)(struct __FBFILTER *fb, float *input, float *output);
    void (*reverse)(float *input, float *output, uint16_t len);
    void (*filter)(struct __FBFILTER *fb, float *input, float *output, uint16_t len);
    void (*fbfilter)(struct __FBFILTER *fb, float *input, float *output, uint16_t len);
} FBFILTER;

void fbfilter_init(FBFILTER *fbf, float *b, float *a, uint8_t order);
#endif
