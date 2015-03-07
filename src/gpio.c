#include <stdint.h>
#include "gpio.h"

static gpio_t* pGPIO = (gpio_t*)GPIO_BASE;

gpio_t* GetGPIO(void) {
  return pGPIO;
}

void GPIOInit(void) {

}
