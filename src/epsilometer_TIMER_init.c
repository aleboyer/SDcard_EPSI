/*
 * epsilometer_TIMER_init.c
 *
 *  Created on: Jan 13, 2017
 *      Author: aleboyer
 */


#include "ep_TIMER_init.h"

/******************************************************************************
 * @name init_TIMER
 * @brief
 *  initialize TIMER0 and TIMER1
 * @details
 * TIMER0 is the Master clock used by the ADC, TIMER1 is a sync signal
 * TIMER0 top is coreclock_cycle is the number of clock cycle that create our ADC Master clock
 * TIMER0 CC2 is the number of cycle to wait before triggering TIMER1. we talk helf of the master clock to be as far as possible of the edge of the master clock
 * TIMER1 top is long enough to let the ADC set up their clock
 * @Author A. Le Boyer
 *****************************************************************************/


void init_TIMER(uint32_t coreclockCycle,uint32_t timer1PhaseShift){

	//set up timer MCLOCK and SYNC
	//configure TIMER0 create and output a
	/* Configure TIMER */
	//TIMER_Init(TIMER0, &timerInit);
//TODO comment out all the lines
	//
	TIMER0->CTRL =(TIMER0->CTRL &~_TIMER_CTRL_CLKSEL_MASK)| TIMER_CTRL_CLKSEL_PRESCHFPERCLK;
	//
	TIMER0->CTRL =(TIMER0->CTRL &~_TIMER_CTRL_PRESC_MASK)| TIMER_CTRL_PRESC_DIV1;
	//
	TIMER0->CTRL =(TIMER0->CTRL &~_TIMER_CTRL_MODE_MASK)| TIMER_CTRL_MODE_UP;
	//
	TIMER0->TOP =(TIMER0->TOP &~_TIMER_TOP_MASK)| TIMER_TOP_TOP_DEFAULT;
    // 2* 16(0xf) cycles of a 20MHz clock = 625kHz => ok for ADCs
	TIMER0->TOP =(TIMER0->TOP &~_TIMER_TOP_MASK)| coreclockCycle;
	//
	TIMER0->CC[2].CTRL =(TIMER0->CC[2].CTRL &~_TIMER_CC_CTRL_MODE_MASK)| TIMER_CC_CTRL_MODE_OUTPUTCOMPARE;
	//
	TIMER0->CC[2].CTRL =(TIMER0->CC[2].CTRL &~_TIMER_CC_CTRL_CMOA_MASK)| TIMER_CC_CTRL_CMOA_TOGGLE;
	//
	TIMER0->CC[2].CCV =(TIMER0->CC[2].CCV &~_TIMER_CC_CCV_CCV_MASK)| TIMER_CC_CCV_CCV_DEFAULT;
	//
	TIMER0->CC[2].CTRL =(TIMER0->CC[2].CTRL &~_TIMER_CC_CTRL_OUTINV_MASK)| TIMER_CC_CTRL_OUTINV;
	//CC2.CCV is triggering TIMER1. we want 90 deg phase shift = 0xf/2 = 0x8
	TIMER0->CC[2].CCV =(TIMER0->CC[2].CCV &~_TIMER_CC_CCV_CCV_MASK)| timer1PhaseShift;  // control the phase of TIMER0
	//
	TIMER0->ROUTE =(TIMER0->ROUTE &~_TIMER_ROUTE_LOCATION_MASK)| TIMER_ROUTE_LOCATION_LOC0;
	//
	TIMER0->ROUTE =(TIMER0->ROUTE &~_TIMER_ROUTE_MASK)| TIMER_ROUTE_CC2PEN;

	//configure TIMER1 to help SYNC the ADC
	TIMER1->CTRL =(TIMER1->CTRL &~_TIMER_CTRL_MODE_MASK)| TIMER_CTRL_MODE_UP;
	//
	TIMER1->CTRL =(TIMER1->CTRL &~_TIMER_CTRL_OSMEN_MASK)| TIMER_CTRL_OSMEN;
	//
    TIMER1->CTRL =(TIMER1->CTRL &~_TIMER_CTRL_SYNC_MASK)| TIMER_CTRL_SYNC_DEFAULT;
	//
	TIMER1->CTRL =(TIMER1->CTRL &~_TIMER_CTRL_CLKSEL_MASK)| TIMER_CTRL_CLKSEL_TIMEROUF;
	//
	TIMER1->CTRL =(TIMER1->CTRL &~_TIMER_CTRL_OSMEN_MASK)| TIMER_CTRL_OSMEN;
	//
	TIMER1->CC[0].CTRL =(TIMER1->CC[0].CTRL &~_TIMER_CC_CTRL_MODE_MASK)| TIMER_CC_CTRL_MODE_OUTPUTCOMPARE;
	//
	TIMER1->CC[0].CTRL =(TIMER1->CC[0].CTRL &~_TIMER_CC_CTRL_OUTINV_MASK)| TIMER_CC_CTRL_OUTINV;
	//
	TIMER1->CC[0].CTRL =(TIMER1->CC[0].CTRL &~_TIMER_CC_CTRL_CMOA_MASK)| TIMER_CC_CTRL_CMOA_TOGGLE;
	//
	TIMER1->CC[0].CTRL =(TIMER1->CC[0].CTRL &~_TIMER_CC_CTRL_INSEL_MASK)| TIMER_CC_CTRL_INSEL_PIN;
	//TIMER1 is going to be 13 cycle of MCLOCK =TIMER0 (625kHz)
//TODO change hard coded 0x13 to a variable in boardSetup
	TIMER1->TOP =(TIMER1->TOP &~_TIMER_TOP_MASK)| 0x13;
	//
	TIMER1->ROUTE =(TIMER1->ROUTE &~_TIMER_ROUTE_MASK)| TIMER_ROUTE_CC0PEN;
	//
	TIMER1->ROUTE =(TIMER1->ROUTE &~_TIMER_ROUTE_LOCATION_MASK)| TIMER_ROUTE_LOCATION_LOC3;


}
