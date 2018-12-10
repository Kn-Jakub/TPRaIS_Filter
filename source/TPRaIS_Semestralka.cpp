/**
 * @file    TPRaIS_Semestralka.cpp
 * @brief   Main application for accelerometer
 * @author	Jakub Pekár
 */

#include <fsl_port.h>
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
	float rawData[3]= {0, 0, 0};
	float filtredData[3] = {0,0,0};
	static float oldFiltredData[3] = {0,0,0};

	void PIT_IRQHandler();
	void PORTA_DriverIRQHandler(void);


} //extern C

void BOARD_INIT();
float filterOneAxis(float input, float oldOutput);
void filter(float* rawData, float *oldData, float* newData);

MMA8451Q* accelerometer;

// *** Main function ***
int main(void)
{

    BOARD_INIT();
    Timer timer;
    accelerometer = new MMA8451Q(ACCELL_ADDRESS);
//    accelerometer->init();
//    accelerometer->freefall();
    accelerometer->tapDetection();
    EnableIRQ(PORTA_IRQn);

//    timer.setTime((uint64_t)DELTA_T * 1000000);
//    timer.starTimer();

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

void PORTA_DriverIRQHandler(void){

	LED_switch(BLUE);
	uint8_t retStatus;
	accelerometer->readRegs(0x0C, &retStatus, 1);
	if(retStatus == 0x04)
	{
		accelerometer->readRegs(0x16, &retStatus, 1);
	}
	PORT_ClearPinsInterruptFlags(PORTA, 1<<BOARD_INITPINS_ACCEL_INT2_PIN);
	PRINTF("FREEEEE FAAALLLL [%d]!!!!!!\n\r", retStatus);
};



void PIT_IRQHandler(){

	PIT_ClearStatusFlags(PIT, kPIT_Chnl_0, kPIT_TimerFlag);

	accelerometer->getAllAxis(rawData);
	filter(rawData, oldFiltredData, filtredData);

	PRINTF("%.3f, %.3f, %.3f \n\r",filtredData[0] , filtredData[1], filtredData[2]);
}

float filterOneAxis(float input, float oldOutput)
{
		return ((1 -alfa) * oldOutput + alfa * input);
}

void filter(float* pRawData, float *pOldData, float* pNewData)
{
	for(int i = 0; i < 3; i++)
	{
		pNewData[i] = (((1 -alfa) * pOldData[i]) + (alfa * pRawData[i]));  //LOW
		pOldData[i] = pNewData[i];
		pNewData[i] = pRawData[i] - pNewData[i];
	}

}
