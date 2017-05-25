/*
 * epsilometer_CMU_init.c
 *
 *  Created on: Jan 13, 2017
 *      Author: aleboyer
 */

#include "em_cmu.h"
#include "ep_CMU_init.h"

volatile uint32_t gulclockset=0;

/******************************************************************************
 * @name CMU_IRQHandler
 * @brief
 *   enable the HF external Xal
 * @details
 *   interrupt enabling an external Xtal. first select the cristal HFXO,
 *   then enable it.
 *   IT IS EXTREMELY IMPORTANT TO SET BY HAND THE FREQUENCY OF THE NEW XTAL IN
 *   CMSIS/EFM32WG/system_efm32wg.c #define EFM32_HFXO_FREQ (20000000UL).
 *   I did not look for a way to make it user friendly :(
 * @Author A. Le Boyer
 *****************************************************************************/


// CMU Interrupt Handler
void CMU_IRQHandler(void)
{
  // Read Interrupt flags
  uint32_t intFlags = CMU_IntGet();

  // Clear interrupt flags register
  CMU_IntClear(CMU_IF_HFXORDY | CMU_IF_HFRCORDY);

    // Check if the Clock Source for External Crystal
  // is Ready or Not?
  if (intFlags & CMU_IF_HFXORDY)
  {
    CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);
    CMU_OscillatorEnable(cmuOsc_HFRCO, false, false);
        //Inform that We are Done
        gulclockset = 1;
  }
}

/******************************************************************************
 * @name init_CMU
 * @brief
 *   initialize the clock management unit.
 * @details
 * start the built in clocks (CMU,GPIO.TIMERs)
 * enable the interrupt that will eneable the external Xtal
 * change control register to digital external clock and select the HFXO (!be more specific on the register)
 * bring the clock on the route 0 pin (!which one?)
 * @Author A. Le Boyer
 *****************************************************************************/

void init_CMU(void){
	/* Use 14 MHZ HFRCO as core clock frequency*/
	CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFRCO);
	CMU_ClockEnable(cmuClock_GPIO, true);
	CMU_ClockEnable(cmuClock_TIMER0, true);
	CMU_ClockEnable(cmuClock_TIMER1, true);



    // Enable the External Oscillator , true for enabling the O and false to not wait
    CMU_OscillatorEnable(cmuOsc_HFXO,true,false);

    // Enable interrupts for HFXORDY
    CMU_IntEnable(CMU_IF_HFXORDY);

    // Enable CMU interrupt vector in NVIC
    NVIC_EnableIRQ(CMU_IRQn);

    // Wait for the HFXO Clock to be Stable - Or infinite in case of error
    while(gulclockset != 1);

    /* set up the 20 MHz clock */
	CMU->CTRL =(CMU->CTRL &~_CMU_CTRL_HFXOMODE_MASK)| CMU_CTRL_HFXOMODE_DIGEXTCLK;
    CMU->CTRL =(CMU->CTRL &~_CMU_CTRL_CLKOUTSEL0_MASK)| CMU_CTRL_CLKOUTSEL0_HFXO;
    CMU->ROUTE =(CMU->ROUTE &~_CMU_ROUTE_CLKOUT0PEN_MASK)| CMU_ROUTE_CLKOUT0PEN;
}
