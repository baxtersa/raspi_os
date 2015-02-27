#include <stdint.h>
#include "system_timer.h"

static system_timer_t* pRpiSystemTimer = (system_timer_t*)RPI_SYSTIMER_BASE;;

system_timer_t* GetSystemTimer(void) {
  return pRpiSystemTimer;
}

void WaitMicroSeconds(uint32_t uMicroSeconds) {
  volatile uint32_t uTimerSeconds = pRpiSystemTimer->counter_lo;

  while((pRpiSystemTimer->counter_lo - uTimerSeconds) < uMicroSeconds) {
    // BLOCK!!!
  }
}
