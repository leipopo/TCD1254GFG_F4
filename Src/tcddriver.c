#include "tcddriver.h"

/*
此驱动将发生tcd需要的三路PWM并将OS信号进行AD转换
*/

/*
此函数通过空指令循环来延时ns纳秒
ARM CortexM3 标称指令速度1.25MIPS/MHz
此代码mcu主频通过HAL_RCC_GetSysClockFreq()获得，为72Mhz
一秒执行1.25*72*10^6条指令
一条指令执行时间为1x10^9/(1.25*72*10^6)ns
所以延时ns纳秒需要执行ns/(10^9/(1.25*72*10^6))条指令
然后向下取整
*/
void delay_ns(int16_t ns)
{
    int32_t mcufreq = HAL_RCC_GetSysClockFreq();
    ns              = (int32_t)((float)ns * (float)mcufreq) * 1.25f / (1000000000.f);
    for (int32_t i = 0; i < ns; i++) {
        __NOP();
    }
}

void scanstart(void)
{
    __HAL_TIM_ENABLE(&icg_tim);
    __HAL_TIM_ENABLE(&sh_tim);
}


