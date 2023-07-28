#include "KF.h"

float kalmanfilter_1th(struct __KF_1thorder *kf, float z)
{
    kf->z = z;
    kf->predict(kf);
    kf->correct(kf);
    return kf->x;
}

__attribute__((weak)) void kalmanfilter_1th_PREDICT(struct __KF_1thorder *kf)
{
    kf->x = (1 - kf->K * kf->H) * kf->x + kf->K * kf->z; // 状态估计
    kf->A = 1 - kf->K * kf->H;                           // 状态转移矩阵更新
    kf->P = kf->A * kf->P * kf->A + kf->Q;               // 先验估计误差协方差更新
}

__attribute__((weak)) void kalmanfilter_1th_CORRECT(struct __KF_1thorder *kf)
{

    kf->K = kf->P * kf->H / (kf->H * kf->P * kf->H + kf->R); // 卡尔曼增益
    kf->x = kf->x + kf->K * (kf->z - kf->H * kf->x);         // 状态估计
    kf->P = (1 - kf->K * kf->H) * kf->P;                     // 状态估计误差协方差更新
}

void kalmanfilter_1th_INIT(struct __KF_1thorder *kf, float Q, float R, float P, float A, float H)
{
    kf->Q       = Q;
    kf->R       = R;
    kf->P       = P;
    kf->A       = A;
    kf->H       = H;
    kf->x       = 0;
    kf->K       = 0;
    kf->z       = 0;
    kf->predict = kalmanfilter_1th_PREDICT;
    kf->correct = kalmanfilter_1th_CORRECT;
}