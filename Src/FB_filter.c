#include "FB_filter.h"

__attribute__((weak)) void forward_filter(FBFILTER *fbf, float *input, float *output, uint16_t len)
{
    for (uint16_t i = 0; i < len; i++) {
        fbf->x[0] = input[i];
        for (uint8_t j = 0; j < fbf->order; j++) {
            fbf->y[0] += fbf->b[j] * fbf->x[j] - fbf->a[j] * fbf->y[j];
        }
        output[i] = fbf->y[0];
        for (uint8_t j = fbf->order - 1; j > 0; j--) {
            fbf->x[j] = fbf->x[j - 1];
            fbf->y[j] = fbf->y[j - 1];
        }
    }
}

__attribute__((weak)) void backward_filter(FBFILTER *fbf, float *output, uint16_t len)
{
    for(uint16_t i = 0; i < len; i++) {
        fbf->x[0] = output[len - i - 1];
        for (uint8_t j = 0; j < fbf->order; j++) {
            fbf->y[0] += fbf->b[j] * fbf->x[j] - fbf->a[j] * fbf->y[j];
        }
        output[len - i - 1] = fbf->y[0];
        for (uint8_t j = fbf->order - 1; j > 0; j--) {
            fbf->x[j] = fbf->x[j - 1];
            fbf->y[j] = fbf->y[j - 1];
        }
    }
}

void fbfilter_init(FBFILTER *fbf, float *b, float *a, uint8_t order)
{
    fbf->b = b;
    fbf->a = a;
    fbf->x = (float *)malloc(sizeof(float) * order);
    fbf->y = (float *)malloc(sizeof(float) * order);
    for (uint8_t i = 0; i < order; i++) {
        fbf->x[i] = 0;
        fbf->y[i] = 0;
    }
    fbf->order    = order;
    fbf->forward  = forward_filter;
    fbf->backward = backward_filter;
}

void fbfilter(FBFILTER *fbf, float *input, float *output, uint16_t len)
{

    fbf->forward(fbf, input, output, len);
    fbf->backward(fbf, output, len);
}
