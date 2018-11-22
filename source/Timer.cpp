/*
 * Timer.cpp
 *
 *  Created on: 19. 11. 2018
 *      Author: jakub
 */

#include <Timer.h>

#include "fsl_debug_console.h"




Timer::Timer(): m_config()
{
	PIT_GetDefaultConfig(&m_config);
	PIT_Init(PIT, &m_config);
}
bool Timer::setTime(uint64_t time_us)
{
	PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, USEC_TO_COUNT(time_us,CLOCK_GetFreq(kCLOCK_BusClk)));
	return true;
}

bool Timer::starTimer()
{
	PIT_EnableInterrupts(PIT, kPIT_Chnl_0, kPIT_TimerInterruptEnable);
	EnableIRQ(PIT_IRQn);
	PIT_StartTimer(PIT, kPIT_Chnl_0);
	return true;
}

Timer::~Timer() {
	// TODO Auto-generated destructor stub
}

