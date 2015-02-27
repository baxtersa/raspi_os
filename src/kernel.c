#include <string.h>
#include <stdlib.h>

#include "gpio.h"

volatile unsigned int* gpio = (unsigned int*)GPIO_BASE;;

void kernel_main(unsigned int r0, unsigned int r1, unsigned int atags) {
  int loop;
  // Allocate block of memory for counters
  unsigned int* counters;

  gpio[LED_GPFSEL] |= (1 << LED_GPFBIT);
  gpio[LED_GPSET] = (1 << LED_GPIO_BIT);

  counters = malloc(1024 * sizeof(unsigned int));

  // Failed to allocate memory
  if (counters == NULL) {
    while (1) {
      // TRAP!!!
    }
  }

  for (loop = 0; loop < 1024; loop++) {
    counters[loop] = 0;
  }

  while(1) {
    gpio[LED_GPSET] = (1 << LED_GPIO_BIT);
    for (counters[0] = 0; counters[0] < 500000; counters[0]++) {

    }
    gpio[LED_GPCLR] = (1 << LED_GPIO_BIT);
    for (counters[0] = 0; counters[0] < 500000; counters[0]++) {

    }
  }

}
