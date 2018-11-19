/*
 * LED.h
 *
 *  Created on: 18. 11. 2018
 *      Author: jakub
 */

#ifndef LED_H_
#define LED_H_
#include <fsl_gpio.h>
#include <pin_mux.h>

typedef enum color{
	RED,
	GREEN,
	BLUE
}COLOR;

void LED_turnOff(COLOR farba){
	switch(farba){
	case RED:
		GPIO_WritePinOutput(BOARD_INITPINS_LED_RED_GPIO, BOARD_INITPINS_LED_RED_PIN, 1);
		break;
	case GREEN:
		GPIO_WritePinOutput(BOARD_INITPINS_LED_GREEN_GPIO, BOARD_INITPINS_LED_GREEN_PIN, 1);
		break;
	case BLUE:
		GPIO_WritePinOutput(BOARD_INITPINS_LED_BLUE_GPIO, BOARD_INITPINS_LED_BLUE_PIN, 1);
		break;
	}
}

void LED_turnOn(COLOR farba){
	switch(farba){
	case RED:
		GPIO_WritePinOutput(BOARD_INITPINS_LED_RED_GPIO, BOARD_INITPINS_LED_RED_PIN, 0);
		break;
	case GREEN:
		GPIO_WritePinOutput(BOARD_INITPINS_LED_GREEN_GPIO, BOARD_INITPINS_LED_GREEN_PIN, 0);
		break;
	case BLUE:
		GPIO_WritePinOutput(BOARD_INITPINS_LED_BLUE_GPIO, BOARD_INITPINS_LED_BLUE_PIN, 0);
		break;
	}
}



#endif /* LED_H_ */
