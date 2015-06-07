#ifndef MMIO_H
#define MMIO_H

#include <stdint.h>

static inline void mmio_write (uint32_t address, uint32_t data) {
    asm volatile ("str %1, %0"
         : "+Qo" (*(volatile uint32_t*) address)
         : "r" (data));
}


static inline uint32_t mmio_read (uint32_t address) {
    uint32_t data;
    asm volatile ("ldr %1, %0"
                  : "+Qo" (*(volatile uint32_t*) address)
                  , "=r" (data));
    return data;
}

#endif