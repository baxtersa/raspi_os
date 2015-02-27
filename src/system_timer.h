#ifndef SYSTEM_TIMER_H
#define SYSTEM_TIMER_H

#include <stdint.h>

#include "base.h"

#define RPI_SYSTIMER_BASE 0x3f003000

typedef struct {
  volatile uint32_t counter_status;
  volatile uint32_t counter_lo;
  volatile uint32_t counter_hi;
  volatile uint32_t compare0;
  volatile uint32_t compare1;
  volatile uint32_t compare2;
  volatile uint32_t compare3;
} system_timer_t;

extern system_timer_t* GetSystemTimer(void);
extern void WaitMicroSeconds(uint32_t uMicroSeconds);

#endif
