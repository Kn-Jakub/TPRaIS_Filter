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

#include "MMA8451Q.h"
#include "LED.h"
#include "Timer.h"

#define MMA8451_I2C_ADDRESS 0x1DU
/*
 * @brief   Application entry point.
 */

void BOARD_INIT()
{
	/* Init board hardware. */
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitBootPeripherals();
	/* Init FSL debug console. */
	BOARD_InitDebugConsole();
}

#if defined(__cplusplus)
extern "C" {
#endif
	volatile int flagIRQ = 0;
	void PIT_IRQHandler(){
		PIT_ClearStatusFlags(PIT, kPIT_Chnl_0, kPIT_TimerFlag);
		flagIRQ = 1;
	}
} //extern C

int main(void)
{
	//pit_config_t config;
	BOARD_INIT();



    LED_turnOn(RED);

    Timer timer;
    MMA8451Q acc(MMA8451_I2C_ADDRESS);

    timer.setTime(100000U);
    timer.starTimer();

    uint8_t ret = acc.getWhoAmI();
    acc.init();
    float x;
    float y;
    float z;

	//PRINTF("Accelerometer(%d) task:: \n\r", ret);
    while(1) {
    	if(flagIRQ == 1){
			x = acc.getAccX();
			y = acc.getAccY();
			z = acc.getAccZ();
			PRINTF("%f, %f, %f \n\r",x,y,z);
			flagIRQ = 0;
    	}
    }
    PRINTF("END OF MAIN\n\r");
    return 0 ;
}
