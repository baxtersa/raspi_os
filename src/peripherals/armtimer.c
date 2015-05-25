#include <stdint.h>
#include "armtimer.h"

static arm_timer_t* pARMTimer = (arm_timer_t*) ARMTIMER_BASE;

arm_timer_t* GetARMTimer (void) {
    return pARMTimer;
}

void ARMTimerInit (void) {
    
}
