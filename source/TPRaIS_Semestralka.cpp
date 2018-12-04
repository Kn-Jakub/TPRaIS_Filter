/**
 * @file    TPRaIS_Semestralka.cpp
 * @brief   Main application for accelerometer
 * @author	Jakub Pekár
 */

#include <stdio.h>
#include <stdint-gcc.h>
#include <tgmath.h>
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
	float angleX,angleY,angleZ = 0;

	void PIT_IRQHandler();
} //extern C

void BOARD_INIT();
float filterOneAxis(float input, float oldOutput);
void filter(float* rawData, float *oldData, float* newData);
float calcAngles(float* data, uint8_t axis);

MMA8451Q* accelerometer;

// *** Main function ***
int main(void)
{

    BOARD_INIT();
    Timer timer;
    accelerometer = new MMA8451Q(ACCELL_ADDRESS);
    accelerometer->init();

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

	PIT_ClearStatusFlags(PIT, kPIT_Chnl_0, kPIT_TimerFlag);

	accelerometer->getAllAxis(rawData);
	filter(rawData, oldFiltredData, filtredData);

	angleX = calcAngles(oldFiltredData, 1);
	angleY = calcAngles(oldFiltredData, 2);
	angleZ = calcAngles(oldFiltredData, 3);

	/* Vypis gravitacne sily */
//	PRINTF("%.3f, %.3f, %.3f \n\r",filtredData[0] , filtredData[1], filtredData[2]);

	/*Vypis zrychlenie na jednolivych osiach*/
//	PRINTF("%.3f, %.3f, %.3f \n\r",oldFiltredData[0] , oldFiltredData[1], oldFiltredData[2]);

	/*Vypis uhol naklonu*/
	PRINTF("%.1f°  %.1f°  %.1f° \n\r",angleX, angleY, angleZ);
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

float calcAngles(float* data, uint8_t axis){
	if(axis == 1){
		return (180/PI) * atanf(data[0]/(sqrtf( powf(data[1],2) + powf(data[2],2) )));
	} else if(axis == 2){
		return (180/PI) * atanf(data[1]/(sqrtf( powf(data[0],2) + powf(data[2],2) )));
	} else {
		return (180/PI) * atanf(data[2]/(sqrtf( powf(data[1],2) + powf(data[0],2) )));
	}
}
