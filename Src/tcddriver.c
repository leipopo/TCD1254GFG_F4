#include "tcddriver.h"


volatile uint16_t CCDDataBuffer[ccdsize];
void scanstart(void)
{
    __HAL_TIM_ENABLE(&icg_tim);
    __HAL_TIM_ENABLE(&sh_tim);
	
}


