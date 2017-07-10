/*
 * epilometer_sampling.c
 *
 *  Created on: Apr 24, 2017
 *      Author: aleboyer
 */


#include "ep_sampling.h"


// flag_sync is just here for devellopment
uint8_t flag_sync = 0;
// for rtc purpose
volatile bool doTemperatureCompensation = true;         // Flags that signal when to do temperature compensation

/******************************************************************************
 * @name MADRE_Sampling
 * @brief
 *   during the sampling we send the data on the SD card. the actual sampling is inside the interrupt c.f analog.c
 * @details
 *	start to send data after 160 sample are available
 *	send a block of 80 sample  1/8 second
 *	once 100 block (every 12.5 second) we sync the sd card which actually right in the file
 *	check if temperature is needed for the rtc
 *
 * @Author A. Le Boyer
 *****************************************************************************/

void MADRE_Sampling() {
	if((pendingSamples % 80 ==0) & (pendingSamples>0)){
		GPIO_PinModeSet(gpioPortA, 13, gpioModePushPull, 1); // and the ADA2200 amp or conductivity sensor
		writeSD();
		GPIO_PinModeSet(gpioPortA, 13, gpioModePushPull, 0); // and the ADA2200 amp or conductivity sensor
		sd_block++;
		if (sd_block % 100 == 0 ) {
			GPIO_PinModeSet(gpioPortA, 14, gpioModePushPull, 1); // and the ADA2200 amp or conductivity sensor
			f_sync(&fsrc);
			GPIO_PinModeSet(gpioPortA, 14, gpioModePushPull, 0); // and the ADA2200 amp or conductivity sensor
			flag_sync=0;
			//send_sdupdate();
		}
	}

//	if((sd_block % 100 == 0 )&(sd_block>0)){
//		send_sdupdate();
//	}
    if (doTemperatureCompensation)	// Perform temperature compensation
  	{
  	    doTemperatureCompensation = false;  // Clear doTemperatureCompensation flag
  	    clockDoTemperatureCompensation();
  	  }
}

/******************************************************************************
 * @name poll_RX
 * @brief
 *   check if a user is trying to talk to the board
 * @details
 *	read the RX buffer
 *
 * @Author A. Le Boyer
 *****************************************************************************/

enum States0 poll_RX(){

	// quick check if the RX buffer empty
	//uint16_t rxData = USART_RxDouble(USART1);
	uint16_t rxData = 0;
	if((USART1->STATUS & USART_STATUS_RXDATAV)){
		rxData = USART_RxDouble(USART1);
		USART1->CMD =(USART1->CMD & ~_USART_CMD_CLEARRX_MASK)|USART_CMD_CLEARRX;

		if (rxData==0x1e1e){
			GPIO_IntDisable(boardSetup_ptr->pinInterupt);
		    GPIO_PinModeSet(gpioPortA, 13, gpioModePushPull, 1); //
		    State=Menu;
		    GPIO_PinModeSet(gpioPortA, 13, gpioModePushPull, 0);
	    }
	}

	return State;



}
