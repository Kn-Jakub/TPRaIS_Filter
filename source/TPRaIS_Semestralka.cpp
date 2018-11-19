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

#include "MMA8451Q.h"
#include "LED.h"

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

int main(void)
{
	BOARD_INIT();

    PRINTF("Hello World\n\r");

    LED_turnOn(RED);

    MMA8451Q acc(MMA8451_I2C_ADDRESS);
    uint8_t ret = acc.getWhoAmI();
    acc.init();
    float x = acc.getAccX();

    /* Enter an infinite loop, just incrementing a counter. */
    char ch = 40;
    while(1) {
    	ch = GETCHAR();
        PUTCHAR(ch);

    }
    PRINTF("END OF MAIN\n\r");
    return 0 ;
}
