/*
 * ep_sampling.h
 *
 *  Created on: Apr 24, 2017
 *      Author: aleboyer
 */

#ifndef INC_EP_SAMPLING_H_
#define INC_EP_SAMPLING_H_


/*energy micro library */
#include "em_rtc.h"
#include "em_usart.h"

/* SD card library */
#include "microsd.h"
#include "ff.h"
#include "diskio.h"


/*local library */
#include "ep_common.h"
#include "ep_analog.h"
#include "ep_coms.h"
#include "clock_tc.h"
#include "ep_SDcard.h"


void MADRE_Sampling();
enum States0 poll_RX();




#endif /* INC_EP_SAMPLING_H_ */
