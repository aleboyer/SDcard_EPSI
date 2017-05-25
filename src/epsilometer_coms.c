/*
 * proto Epsilometer_coms.c
 *
 *  Created on: May 24, 2016
 *      Author: leumas64
 */

#include "ep_coms.h"
#include "ep_common.h"
#include "em_usart.h"
#include "em_cmu.h"
#include "efm32wg_uart.h"



/******************************************************************************
 * @name UART_Setup
 * @brief
 *  set up the communication with the RS232
 * @details
 *	enable USART1 clock
 *	switch on the TX and RX GPIO
 *	configure USART1: baudrate ....
 *	set up an interrupt to TX the data
 *	lower the priority of the interrupt so it is lower than tha sampling interrupt
 *
 * @Author A. Le Boyer
 *****************************************************************************/

void UART_Setup() {
	/*UART1 shit*/
	CMU_ClockEnable(cmuClock_USART1, true); // Enable clock for USART1 module
	GPIO_PinModeSet(gpioPortD, 7, gpioModePushPull, 1); // TX
	GPIO_PinModeSet(gpioPortD, 6, gpioModeInput, 0); // RX

	USART_InitAsync_TypeDef usartInitUSART1 = {
		.enable = usartDisable, 					// Initially disabled
		.refFreq = 0,								// configured reference frequency
		.baudrate = boardSetup_ptr->usart_baudrate, 				    // Baud rate defined in common.h
		.oversampling = usartOVS16, 				// overSampling rate x16
		.databits = USART_FRAME_DATABITS_EIGHT, 	// 8-bit frames
		.parity = USART_FRAME_PARITY_NONE,			// parity - none
		.stopbits = USART_FRAME_STOPBITS_ONE,		// 1 stop bit
	};

	/*Initialize UART registers*/
	USART_InitAsync(USART1, &usartInitUSART1);

	USART1 -> ROUTE = USART_ROUTE_RXPEN | USART_ROUTE_TXPEN
			| USART_ROUTE_LOCATION_LOC2;

	/* Inform NVIC of IRQ changes*/
	NVIC_ClearPendingIRQ(USART1_TX_IRQn);
	NVIC_EnableIRQ(USART1_TX_IRQn);

	USART_Enable(USART1, usartEnable);
	NVIC_SetPriority(USART1_TX_IRQn, 1);
}


/******************************************************************************
 * @name UART_Setup
 * @brief
 *  send 3 bytes through TX
 * @details
 *	send 3 bytes block when TX buffer (2 bytes for 2 FIFO level + 1 bytes for shift register)
 *
 * @Author A. Le Boyer
 *****************************************************************************/

void USART1_TX_IRQHandler(void)
{
//TODO change to state machine
	dataLen = pendingSamples*byte_per_sample-tx_bytes_sent;

	switch (dataLen){
		case 0:
			USART_IntDisable(USART1, USART_IEN_TXBL);
			USART_Tx(USART1, 0x1e);

			break;
		case 1:
			USART_Tx(USART1, dataBuffer[tx_bytes_sent % buffer_size]);
			tx_bytes_sent++;
			break;
		default:
			for(int i=0;i<2;i++){
				/* Transmit pending character */
				USART_Tx(USART1, dataBuffer[tx_bytes_sent % buffer_size]);
				tx_bytes_sent++;
			}
			break;
	}

}






