#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <stdint.h>
#include "base.h"

#define INTERRUPT_CONTROLLER_BASE 0x3F00B200

#define BASIC_ARM_TIMER_IRQ (1 << 0)
#define BASIC_ARM_MAILBOX_IRQ (1 << 1)
#define BASIC_ARM_DOORBELL_0_IRQ (1 << 2)
#define BASIC_ARM_DOORBELL_1_IRQ (1 << 3)
#define BASIC_GPU_0_HALTED_IRQ (1 << 4)
#define BASIC_GPU_1_HALTED_IRQ (1 << 5)
#define BASIC_ACCESS_ERROR_1_IRQ (1 << 6)
#define BASIC_ACCESS_ERROR_0_IRQ (1 << 7)

typedef struct {
  volatile uint32_t m_uIRQBasicPending;
  volatile uint32_t m_uIRQPending1;
  volatile uint32_t m_uIRQPending2;
  volatile uint32_t m_uFIQControl;
  volatile uint32_t m_uEnableIRQs1;
  volatile uint32_t m_uEnableIRQs2;
  volatile uint32_t m_uEnableBasicIRQs;
  volatile uint32_t m_uDisableIRQs1;
  volatile uint32_t m_uDisableIRQs2;
  volatile uint32_t m_uDisableBasicIRQs;
} irq_controller_t;

extern irq_controller_t* GetIRQController(void);

#endif
