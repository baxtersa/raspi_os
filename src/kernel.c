#include <string.h>
#include <stdlib.h>

#include "gpio.h"
#include "armtimer.h"
#include "system_timer.h"
#include "interrupts.h"

void kernel_main(unsigned int r0, unsigned int r1, unsigned int atags) {

  int ilit = 0;

  // Enable GPIO output
  GetGPIO()->m_rGPFSEL4 |= (1 << LED_GPFBIT);

  // Enable the timer interrupt IRQ
  GetIRQController()->m_uEnableBasicIRQs = BASIC_ARM_TIMER_IRQ;

  // Setup the system timer interrupt
  // Timer frequency = Clk/256 * 0x400
  GetARMTimer()->m_uLoad = 0x400;

  // Setup the ARM Timer
  GetARMTimer()->m_uControl = ARMTIMER_CTRL_23BIT
    | ARMTIMER_CTRL_ENABLE
    | ARMTIMER_CTRL_INT_ENABLE
    | ARMTIMER_CTRL_PRESCALE_256;

  // Enable interrupts!
  _enable_interrupts();

  // Never exit
  while(1) {
    // TRAP!!!
  }
}
