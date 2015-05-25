#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "drivers/gpio.h"
#include "peripherals/armtimer.h"
#include "system_timer.h"
#include "interrupts.h"
#include "framebuffer.h"

void kernel_main (unsigned int r0, unsigned int r1, unsigned int atags) {
    int ilit = 0;
    
    // Enable GPIO output
    GetGPIO ()->m_rGPFSEL4 |= (1 << LED_GPFBIT);
    
    // Enable the timer interrupt IRQ
    GetIRQController ()->m_uEnableBasicIRQs = BASIC_ARM_TIMER_IRQ;
    
    // Setup the system timer interrupt
    // Timer frequency = Clk/256 * 0x400
    GetARMTimer ()->m_uLoad = 0x400;
    
    // Setup the ARM Timer
    GetARMTimer ()->m_uControl = ARMTIMER_CTRL_23BIT
	| ARMTIMER_CTRL_ENABLE
	| ARMTIMER_CTRL_INT_ENABLE
	| ARMTIMER_CTRL_PRESCALE_256;
    
    // Enable interrupts!
    _enable_interrupts ();
    
    // Load framebuffer info and initialize
    FrameBufferInit ();

    if (!FrameBufferIsInitialized ()) {
	while (1) {
	    printf ("Frame buffer initialization failed!\n");
	    // TRAP!!!
	}
    }
    
    ClearFrameBuffer (MakeColor (0, 0xff, 0xff));
    SetBackgroundColor (MakeColor (0xff, 0xff, 0xff));
    SetForegroundColor (MakeColor (0, 0, 0xff));
    HorizontalLine (MakeColor (0x23, 0x81, 0xF2), 0, 800, 300);
  
    // Never exit
    while (1) {
	printf ("Trapped in kernel.c!\n");
	// TRAP!!!
    }
}
