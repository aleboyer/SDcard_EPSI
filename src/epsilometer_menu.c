/*
 * epsilometer_menu.c
 *
 *  Created on: Apr 24, 2017
 *      Author: aleboyer
 */



#include "ep_menu.h"


/******************************************************************************
 * @name MADRE_menu
 * @brief
 *   interaction with the user. User can my python script.
 * @details
 *	define a character chain giving the user options.
 *	If user send 1 sent the user into the change config mode
 *	If user send 2 resume sampling with the new configuration if any
 *
 * @Author A. Le Boyer
 *****************************************************************************/


void MADRE_menu(void){

    // define a sentence to send to the user
	char buf[100];
	uint8_t  rxData16  = 0;
	sprintf(buf,"ACTIONS: \n 1- CHANGE CONFIG \n 2- RESUME SAMPLING\n");

	// USART_RxDouble have while loop which will hold on until a bytes is going to come.
	// giving us the time to clean serial buffer in python. This is not very elegant
	// but I already spend almost a whole day to figure out something "smart"

	// menu selection
	while((rxData16==0) | (rxData16>2)){
		// send the user action option through TX.
		for(int i=0;i<49;i++){
			USART_TxDouble(USART1,buf[i]);
		}
		rxData16 = USART_Rx(USART1);
		if(rxData16==1) {
			break;
		}
	}

	// allow user to cahnge the config
	if (rxData16==1){
			GPIO_PinModeSet(gpioPortA, 13, gpioModePushPull, 1); //
			MADRE_Change_Config();
			GPIO_PinModeSet(gpioPortA, 13, gpioModePushPull, 0);
	}
}


/******************************************************************************
 * @name MADRE_Change_Config
 * @brief
 *   TODO
 * @details
 *	TODO
 * @Author A. Le Boyer
 *****************************************************************************/
void MADRE_Change_Config(void) {

	char buf[100];
	uint16_t rxData1 = 0;
	uint16_t rxData2 = 0;
	uint32_t param;
	sprintf(buf,"Number of sensor (default:%i,current:%i): \n",8,(int) boardSetup_ptr->number_sensor);
	for(int i=0;i<49;i++){
		USART_Tx(USART1,buf[i]);
	}

	rxData1 = USART_RxDouble(USART1);
	rxData2 = USART_RxDouble(USART1);
	param = (int32_t) rxData1 <<16;
	param = (int32_t) rxData2;

	sprintf(buf,"new number of sensor %i \n",param);
	for(int i=0;i<49;i++){
		USART_Tx(USART1,buf[i]);
	}
	rxData1 = USART_RxDouble(USART1);
}



/******************************************************************************
 * @name MADRE_Config
 * @brief
 *   Configure and switch on the needed part on MADRE
 * @details
 *   init the clock management
 *   init the GPIO port (switch the ADC)
 *   set up UART the communication port between the ADC and the MCU
 *   set up the GPIO interrupt controled by the ADC
 *   configure the ADC
 *   set the main buffer size
 *   initialize the TIMERS
 *   initialize the RS232 UART
 *   initialize the SD card
 *   open a file on the SD card to start the sampling
 *
 * @Author A. Le Boyer
 *****************************************************************************/
void MADRE_Config(void) {
	/* Use 20 MHZ HFRCO as core clock frequency*/
    /* use of an interrupt on CMU to trigger the Oscillator when ready */
    init_CMU();
	// define GPIO pin mode for ADC and the 485, PA2 to send MCLOCK (for ADCs), and PE7 to send SYNC
    init_GPIO();

	main_calendar_setup();


	init_SPI();     // Initialize the SPI interface for the Analog front end
	AD7124_Reset(); // Place devices into a known state
	setupGpioInt(); // Setup GPIO Interrupt but leave disabled
	define_ADC_configuration(); // set up and send the registers for the ADC

	// set variable
	byte_per_sample       = boardSetup_ptr->number_sensor*3+boardSetup_ptr->timestamp_flag*4;          // 3 bytes per channel * number of channel. (ADC resolution 24 bits = 3 bytes)
	buffer_size       = boardSetup_ptr->maximum_samples*byte_per_sample;                                //
	uint32_t coreclock_cycle   = boardSetup_ptr->core_clock/boardSetup_ptr->master_clock_frequency/2-1;    //
	uint32_t timer1_phase_shift = .5 * coreclock_cycle;

	// uint32 array where data are stored and from where data are sent to the serial port
    dataBuffer       = malloc(sizeof(uint8_t)*buffer_size);

    //set up timer MCLOCK and SYNC
	init_TIMER(coreclock_cycle,timer1_phase_shift);

    UART_Setup();
	// initialize SD: mount the driver, open file, write a header
	initSD();

	// open SD file
	res = f_open(&fsrc, filename, FA_OPEN_APPEND | FA_WRITE);
}

/******************************************************************************
 * @name MADRE_resume_sampling
 * @brief
 *   Re sync the ADC and enable the GPIO interrupt
 * @details
 *   initialize the sampling, tx and SD counters.
 *   Re sync the ADC and enable the GPIO interrupt
 *   change the state of the board
 *
 * @Author A. Le Boyer
 *****************************************************************************/

enum States0 MADRE_resume_sampling(void){

    // define foreground and background counter
	volatile uint32_t pendingSamples = 0; 	 // samples from ADC
    volatile uint32_t txSentBytes    = 0; 	 // sample send by TX
    volatile uint32_t SDsample       = 0;    // sample on SD


	// start conversion: send SYNC signal and enable the sampling interrupt
	//TODO change sensors[0] to sensors(master) where master is define in common.h. imply changes in epsilometer_analog.c

    AD7124_StartConversion(sensors[0]);
	State = Sampling;
	return State;
}

