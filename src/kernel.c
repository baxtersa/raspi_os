#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "drivers/gpio.h"
#include "peripherals/armtimer.h"
#include "system_timer.h"
#include "interrupts.h"
#include "framebuffer.h"

void kernel_main(unsigned int r0, unsigned int r1, unsigned int atags) {
    // Bail if r0 != 0, as ATAG docs say it must be 0
    if (r0 != 0) {
	return;
    }
    
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
    
    // Load framebuffer info and initialize
    FrameBufferInit();
    
    ClearFrameBuffer(MakeColor(0, 0xff, 0));
    SetBackgroundColor(MakeColor(0xff, 0xff, 0xff));
    SetForegroundColor(MakeColor(0, 0, 0xff));
    
    // Never exit
    while(1) {
	printf("Trapped in kernel.c!\n");
	// TRAP!!!
    }
}
