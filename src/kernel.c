#include <string.h>
#include <stdlib.h>

#include "gpio.h"
#include "system_timer.h"

volatile unsigned int* gpio = (unsigned int*)GPIO_BASE;;

void kernel_main(unsigned int r0, unsigned int r1, unsigned int atags) {
  int loop;

  gpio[LED_GPFSEL] |= (1 << LED_GPFBIT);

  while(1) {
    gpio[LED_GPSET] = (1 << LED_GPIO_BIT);
    WaitMicroSeconds(500000);

    gpio[LED_GPCLR] = (1 << LED_GPIO_BIT);
    WaitMicroSeconds(500000);

  }
}
