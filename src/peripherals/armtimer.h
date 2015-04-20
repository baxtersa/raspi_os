#ifndef ARMTIMER_H
#define ARMTIMER_H

#include <stdint.h>
#include "../base.h"

#define ARMTIMER_BASE 0x3F00B400

#define ARMTIMER_CTRL_23BIT (1 << 1)

#define ARMTIMER_CTRL_PRESCALE_1 (0 << 2)
#define ARMTIMER_CTRL_PRESCALE_16 (1 << 2)
#define ARMTIMER_CTRL_PRESCALE_256 (2 << 2)

#define ARMTIMER_CTRL_INT_ENABLE (1 << 5)
#define ARMTIMER_CTRL_INT_DISABLE (0 << 5)

#define ARMTIMER_CTRL_ENABLE (1 << 7)
#define ARMTIMER_CTRL_DISABLE (0 << 7)

typedef struct {
  volatile uint32_t m_uLoad,
    m_uValue,
    m_uControl,
    m_uIRQClear,
    m_uRAWIRQ,
    m_uMaskedIRQ,
    m_uReload,
    m_uPreDivider,
    m_uFreeRunningCounter;
} arm_timer_t;

extern arm_timer_t* GetARMTimer(void);
extern void ARMTimerInit(void);

#endif
