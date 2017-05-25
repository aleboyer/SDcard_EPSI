/*
 * ep_common.h
 *
 *  Created on: Jun 20, 2016
 *      Author: leumas64
 *      modified Dec 22 2016 A LeBoyer
 */

#ifndef EP_COMMON_H_
#define EP_COMMON_H_


/******************************************************************************/
/***************************** Include Files **********************************/
/******************************************************************************/

#include "stdint.h"
#include "stdlib.h"
#include "ff.h"

/*************************************************************************/
/* define epsilometer parameters info to be put into the header data   */
/*************************************************************************/

//#define CHANNELS {&FP07_1, &SHR_1, &SHR_2, &CON_1, &AXL_X, &AXL_Y,&FP07_2, &AXL_Z}


typedef struct epsiSetup {
	uint32_t 	core_clock;             // frequency of the cristal
	uint32_t 	number_sensor;          // number of sensor used. It will always go from 1 to numSensor in the list define in the main
	uint32_t	maximum_samples;        // max number of sample in the dataBuffer. A sample is all the channels data + timestamp if timeStampflag=1
	uint32_t    pinInterupt;            // GPIO used to trigger the sample interrupt.
	uint32_t	blocksize;              // Is it used ?
	uint32_t	sample_frequency;       // sample frequency in Hz used in epsilometer_analog.c define_ADC_configuration
	uint32_t    master_clock_frequency; // frequency in Hz of the clock used by the ADC
	uint32_t    timer1Sync;             // so far number of cycle of the master clock to trigger TIMER1 which will sync all the ADC togther
	uint32_t	spi_baudrate;           // baudrate of the spi bus: ADC to MCU communication
	uint32_t	usart_baudrate;         // baudrate ot the RS232 TX/RX
	uint32_t	timestamp_flag;         // flag to add a time stamp in with the data
} epsiSetup, *epsiSetupPtr;


// blockSize size of streamed data in samples number
// Max sample is import

//TODO check the memory use and maxsample, do not forget that SD write must be 512 so we need at least 2*512 bytes to be "safe" (1024/8channel = 128)


#define EPSI_SETUP_DEFAULT       \
{20000000,                       \
8,                               \
320,                             \
0x200,                           \
1,                               \
60,                              \
614400,                          \
0x13,                            \
1000000,                         \
460800,                          \
1                                \
}


// define the name of the file we open on the SD card
#define FILENAME "ep_test.bin"
TCHAR filename[256];
FIL fsrc;					// File objects


// define the different state of MADRE (not use in this Application note)
enum States0 {
  Sampling,
  Menu
};
enum States0 State;


/*************************************************************************/
/**   define global external values  **********************************************/
/*************************************************************************/

volatile int32_t byte_per_sample;     // number of bytes per Sample
volatile uint32_t buffer_size;        // numChannel*maxSamples

/*************************************************************************/
/* define circular buffer for the samples.
 * with timeStampFlag 1 One sample is numChannel * byteSample + 1 byteSample long
 * with timeStampFlag 0 One sample is numChannel * byteSample long
 * the buffer is maxSample long
 */
/*************************************************************************/

volatile uint8_t * dataBuffer;      // main buffer. reord all the data. Its size is define in MADRE_config
volatile uint8_t * channelSample;   // ??
volatile uint32_t pendingSamples;   // counter for samples gathered from the ADC
volatile uint32_t tx_bytes_sent;    // number of bytes sent
volatile uint32_t sd_block;         // number of "block" sent on the SD
volatile uint32_t gulclockset;      // used in the interrupt enabling the HFXO

extern epsiSetupPtr boardSetup_ptr; // default user parameter


#endif /* EP_COMMON_H_ */
