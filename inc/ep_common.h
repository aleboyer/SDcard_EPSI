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
	uint32_t 	coreClock;       // frequency of the cristal
	uint32_t 	numSensor;       // number of sensor used. It will always go from 1 to numSensor in the list define in the main
	uint32_t	maxSamples;      // max number of sample in the dataBuffer. A sample is all the channels data + timestamp if timeStampflag=1
	uint32_t    pinInterupt;
	uint32_t	blockSize;
	uint32_t	sampleFreq;
	uint32_t    MclockFreq;
	uint32_t    timer1Sync;
	uint32_t	spiBaudrate;
	uint32_t	usartBaudrate;
	uint32_t	timeStampFlag;
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


#define FILENAME "ep_test.bin"
TCHAR filename[256];
FIL fsrc;					// File objects



/*************************************************************************/
/**   define global external values  **********************************************/
/*************************************************************************/

uint32_t byteSample;        // number of bytes per Sample
uint32_t bufferSize;        // numChannel*maxSamples

/*************************************************************************/
/* define circular buffer for the samples.
 * with timeStampFlag 1 One sample is numChannel * byteSample + 1 byteSample long
 * with timeStampFlag 0 One sample is numChannel * byteSample long
 * the buffer is maxSample long
 */
/*************************************************************************/

//volatile uint8_t * dataBuffer;
uint8_t * dataBuffer;
volatile uint8_t * channelSample;
extern volatile uint32_t pendingSamples; // counter for samples gathered from the ADC
extern volatile uint32_t txSentBytes;
volatile uint32_t SDblock;              // counter in the foreground SD
volatile uint32_t flagSync;              // flag to reset pending sample in the interrupt.
extern epsiSetupPtr boardSetup_ptr;
volatile uint32_t gulclockset;


//volatile uint32_t samplesSent;           // counter for samples sent by TX

#endif /* EP_COMMON_H_ */
