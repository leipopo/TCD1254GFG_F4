#ifndef __FB_FILTER_H
#define __FB_FILTER_H
#include <stdlib.h>
#include "stdint.h"

typedef struct __FBFILTER {
    float *b;
    float *a;
    float *x;
    float *y;
    float order;
    void (*forward)(struct __FBFILTER *fb, float *input, float *output, uint16_t len);
    void (*backward)(struct __FBFILTER *fb, float *output, uint16_t len);
} FBFILTER;

void fbfilter_init(FBFILTER *fbf, float *b, float *a, uint8_t order);
void fbfilter(FBFILTER *fbf, float *input, float *output, uint16_t len);
#endif
