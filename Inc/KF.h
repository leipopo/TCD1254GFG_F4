#ifndef _KF_H_
#define _KF_H_

typedef struct __KF_1thorder {
    float x; // 状态估计
    float P; // 状态估计误差协方差
    float Q; // 状态噪声协方差
    float R; // 观测噪声协方差
    float K; // 卡尔曼增益
    float A; // 状态转移矩阵
    float H; // 观测矩阵
    float z; // 观测值

    // void (*init)(struct __KF_1thorder *kf, float Q, float R, float P, float A, float H);
    void (*predict)(struct __KF_1thorder *kf);
    void (*correct)(struct __KF_1thorder *kf);

} KF_1thorder;

float kalmanfilter_1th(struct __KF_1thorder *kf, float z);
void kalmanfilter_1th_INIT(struct __KF_1thorder *kf, float Q, float R, float P, float A, float H);

#endif
