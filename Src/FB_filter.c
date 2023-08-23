#include "FB_filter.h"

__attribute__((weak)) void zi_calc(FBFILTER *fbf)
{
    for (uint8_t i = 0; i < fbf->order - 1; i++) {
        fbf->zi[i] = 0;
    }
    for (int16_t i = fbf->order - 2; i >= 0; i--) {
        for (uint8_t j = 0; j < fbf->order; j++) {
            fbf->zi[i] += fbf->b[j];
        }
        for (uint8_t j = 1; j < fbf->order; j++) {
            if (j <= i) {
                fbf->zi[i] -= fbf->a[j];
            } else if (j != i + 1) {
                fbf->zi[i] -= fbf->a[j] * fbf->zi[j - 1];
            }
        }
        fbf->zi[i] = (fbf->zi[i] - 1) / fbf->a[i + 1];
    }
}

__attribute__((weak)) void initvalue_calc(FBFILTER *fbf, float *input, float *output)
{
    // for (uint16_t i = 0; i < fbf->order - 1; i++) {
    //     if (i == 0) {
    //         output[i] = fbf->zi[fbf->order - i - 2] * input[i];
    //     } else {
    //         output[i] = fminf(fbf->zi[fbf->order - i - 2] * input[i], output[i - 1]);
    //     }

    // }
    // for (uint16_t i=0;i<fbf->order-1;i++){
    //     fbf->x[i + 1] = input[fbf->order - i - 2];
    //     fbf->y[i + 1] = output[fbf->order - i - 2];
    // }
    for (uint16_t i = 0; i < fbf->order - 1; i++) {
    output[i] = input[0];
}
    for (uint16_t i = 0; i < fbf->order - 1; i++) {
        fbf->x[i + 1] = input[fbf->order - i - 2];
        fbf->y[i + 1] = output[fbf->order - i - 2];
    }
}

__attribute__((weak)) void reverse_arr(float *input, float *output, uint16_t len)
{
    float temp;
    for (uint16_t i = 0; i < len / 2; i++) {
        temp                = input[i];
        output[i]           = input[len - i - 1];
        output[len - i - 1] = temp;
    }
}

__attribute__((weak)) void lpfilter(FBFILTER *fbf, float *input, float *output, uint16_t len)
{

    for (uint16_t i = 2; i < len; i++) {
        fbf->x[0] = input[i];
        fbf->y[0] = fbf->b[0] * fbf->x[0];
        for (uint8_t j = 1; j < fbf->order; j++) {
            fbf->y[0] += fbf->b[j] * fbf->x[j] - fbf->a[j] * fbf->y[j];
        }
        output[i] = fbf->y[0];
        for (uint8_t j = fbf->order - 1; j > 0; j--) {
            fbf->x[j] = fbf->x[j - 1];
            fbf->y[j] = fbf->y[j - 1];
        }
    }
}

__attribute__((weak)) void filtfilt(FBFILTER *fbf, float *input, float *output, uint16_t len)
{

    fbf->initvalue_calc(fbf, input, output);
    fbf->filter(fbf, input, output, len);
    fbf->reverse(output, output, len);
    fbf->initvalue_calc(fbf, output, output);
    fbf->filter(fbf, output, output, len);
    fbf->reverse(output, output, len);
}

void fbfilter_init(FBFILTER *fbf, float *b, float *a, uint8_t order)
{
    fbf->b     = b;
    fbf->a     = a;
    fbf->order = order + 1;
    fbf->x     = (float *)malloc(sizeof(float) * fbf->order);
    fbf->y     = (float *)malloc(sizeof(float) * fbf->order);

    fbf->zi             = (float *)malloc(sizeof(float) * (fbf->order - 1));
    fbf->zi_calc        = zi_calc;
    fbf->initvalue_calc = initvalue_calc;
    fbf->reverse        = reverse_arr;
    fbf->filter         = lpfilter;
    fbf->fbfilter       = filtfilt;
    fbf->zi_calc(fbf);
}
