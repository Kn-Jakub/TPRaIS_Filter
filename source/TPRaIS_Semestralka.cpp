/**
 * @file    TPRaIS_Semestralka.cpp
 * @brief   Main application for accelerometer
 * @author	Jakub Pekár
 */

#include <stdio.h>
#include <stdint-gcc.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
#include "fsl_pit.h"


#include "include/MMA8451Q.h"
#include "include/LED.h"
#include "include/Timer.h"

#define ACCELL_ADDRESS 0x1DU

#define PI 		(float)		3.1415926535897
#define Fc 		(float)		5
#define DELTA_T (float)		0.001

static const float alfa = (2*PI*DELTA_T*Fc)/((2*PI*DELTA_T*Fc) + 1) ; // low pass

// ** Predefinitions **

#if defined(__cplusplus)
extern "C" {
#endif
	volatile int flagIRQ = 0;
	void PIT_IRQHandler();
} //extern C

void BOARD_INIT();
float filterOneAxis(float input, float oldOutput);
void filter(float* rawData, float *oldData, float* newData);

MMA8451Q* accelerometer;

// *** Main function ***
int main(void)
{
    Timer timer;
    accelerometer = new MMA8451Q(ACCELL_ADDRESS);

    BOARD_INIT();

    timer.setTime((uint64_t)DELTA_T * 1000000);
    timer.starTimer();

    while(1)
    {}

    return 0 ;
}

void BOARD_INIT()
{
	/* Init board hardware. */
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitBootPeripherals();
	/* Init FSL debug console. */
	BOARD_InitDebugConsole();
}

void PIT_IRQHandler(){
	float rawData[3]= {0, 0, 0};
	float filtredData[3] = {0,0,0};
	static float oldFiltredData[3] = {0,0,0};

	PIT_ClearStatusFlags(PIT, kPIT_Chnl_0, kPIT_TimerFlag);

	accelerometer->getAllAxis(rawData);
	filter(rawData, oldFiltredData, filtredData);

	PRINTF("%.3f, %.3f, %.3f \n\r",filtredData[0] , filtredData[1], filtredData[2]);
}

float filterOneAxis(float input, float oldOutput)
{
		return ((1 -alfa) * oldOutput + alfa * input);
}

void filter(float* rawData, float *oldData, float* newData)
{
	for(int i = 0; i < 3; i++)
	{
		newData[i] = (((1 -alfa) * oldData[i]) + (alfa * rawData[i]));  //LOW
		oldData[i] = newData[i];
		newData[i] = rawData[i] - newData[i];
	}

}
