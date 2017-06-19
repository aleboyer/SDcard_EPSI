/**************************************************************************//**
 * @file epsilometer_main.c
 * @brief include, definition, call and main loop needed to operate the MADRE
 * @author A. Le Boyer
 * @version 1.0
 ******************************************************************************
 * @section License
 * <b>(C) Copyright 2014 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensed under the Silicon Labs Software License Agreement. See 
 * "http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt"  
 * for details. Before using this software for any purpose, you must agree to the 
 * terms of that agreement.
 *
 ******************************************************************************/

/*local library */
#include "ep_common.h"
#include "ep_sampling.h"
#include "ep_menu.h"


/******************************************************************************/
/*************************** List of Possible Channels / Sensor *******************************/
/******************************************************************************/

epsiSetup boardsetup = EPSI_SETUP_DEFAULT;
epsiSetupPtr boardSetup_ptr =  &boardsetup;

//TODO find a way to be able to select only the sensor we want to use. It will interact with GPIO and ADC config
SensorSpec fp07_1 = {"T1", AD7124_RESET_DEFAULT, {gpioPortA, 0}};  //PA0
SensorSpec fp07_2 = {"T2", AD7124_RESET_DEFAULT, {gpioPortC, 5}};  //PC5
SensorSpec shr_1  = {"S1", AD7124_RESET_DEFAULT, {gpioPortC, 6}};  //PC6
SensorSpec shr_2  = {"S2", AD7124_RESET_DEFAULT, {gpioPortC, 7}};  //PC7
SensorSpec con_1  = {"C1", AD7124_RESET_DEFAULT, {gpioPortC, 12}}; //PC12
SensorSpec ax     = {"AX", AD7124_RESET_DEFAULT, {gpioPortC, 14}}; //PC14
SensorSpec ay     = {"AY", AD7124_RESET_DEFAULT, {gpioPortC, 15}}; //PC15
SensorSpec az     = {"AZ", AD7124_RESET_DEFAULT, {gpioPortA, 1}};  //PA1


//TODO I am lost in the volatile side of life. Check if all of them are needed
// create a list of all the sensor and the configuration of their analog front end
volatile SensorSpec_ptr sensors[8]={&fp07_1, &fp07_2, &shr_1, &shr_2, &con_1, &ax, &ay, &az};


/******************************************************************************
 * @brief
 *   Main function.
 * @details
 *   Initialization of the board with CHIP_Init, MADRE_Config.
 *   start sampling by triggering an interrupt with MADRE_resume_sampling
 *****************************************************************************/
int main(void) {
    /* Initialize chip - handle erratas */
    CHIP_Init();

	MADRE_Config();
	State=MADRE_resume_sampling();
	/****************************************************************
	 * Primitive Sampling routine
	 ****************************************************************/
	while (1) {

		switch (State){
			case Sampling:
				MADRE_Sampling();
				//State=poll_RX();
				break;
			case Menu:
				MADRE_menu();
				MADRE_Config();
				State=MADRE_resume_sampling();
				break;
		}
	}// end while loop
}// end main
