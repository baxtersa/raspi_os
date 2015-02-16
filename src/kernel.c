#if !defined(__cplusplus)
#include <stdbool.h>
#endif
#include <stddef.h>
#include <stdint.h>


static inline void mmio_write(uint32_t reg, uint32_t data) {
  uint32_t* ptr = (uint32_t*) reg;
  asm volatile("str %[data], [%[reg]]" : : [reg]"r"(ptr), [data]"r"(data));
}


static inline uint32_t mmio_read(uint32_t reg) {
  uint32_t *ptr = (uint32_t*) reg;
  uint32_t data;
  asm volatile("ldr %[data], [%[reg]]" : [data]"=r"(data) : [reg]"r"(ptr));
  return data;
}


// Loop <delay times in a way that the compiler won't optimize away. */
static inline void delay(int32_t count) {
  asm volatile("__delay_%=: subs %[count], %[count], #1; bne __delay_%=\n"
	       : : [count]"r"(count) : "cc");
}


size_t strlen(const char* str) {
  size_t ret = 0;
  while (str[ret] != 0) {
    ret++;
  }
  return ret;
}


enum {
  // The GPIO registers base address.
  GPIO_BASE = 0x20200000,

  // The offsets for each register.

  // Controls actuation of pull up/down to ALL GPIO pins.
  GPPUD = (GPIO_BASE + 0x94),

  // Controls actuation of pull up/down for specific GPIO pin.
  GPPUDCLK0 = (GPIO_BASE + 0x98),

  // The base address for UART.
  UART0_BASE = 0x20201000,

  // The offsets for each register for the UART.
  UART0_DR = (UART0_BASE + 0x00),
  UART0_RSRECR = (UART0_BASE + 0x04),
  UART0_FR = (UART0_BASE + 0x18),
  UART0_ILPR = (UART0_BASE + 0x20),
  UART0_IBRD = (UART0_BASE + 0x24),
  UART0_FBRD = (UART0_BASE + 0x28),
  UART0_LCRH = (UART0_BASE + 0x2C),
  UART0_CR = (UART0_BASE + 0x30),
  UART0_IFLS = (UART0_BASE + 0x34),
  UART0_IMSC = (UART0_BASE + 0x38),
  UART0_RIS = (UART0_BASE + 0x3C),
  UART0_MIS = (UART0_BASE + 0x40),
  UART0_ICR = (UART0_BASE + 0x44),
  UART0_DMACR = (UART0_BASE + 0x48),
  UART0_ITCR = (UART0_BASE + 0x80),
  UART0_ITIP = (UART0_BASE + 0x84),
  UART0_ITOP = (UART0_BASE + 0x88),
  UART0_TDR = (UART0_BASE + 0x8C),
};


void uart_init() {
  // Disable UART0
  mmio_write(UART0_CR, 0x00000000);
  // Setup the GPIO pin 14 && 15.

  // Disable pull up/down for all GPIO pins & delay for 150 cycles
  mmio_write(GPPUD, 0x00000000);
  delay(150);

  // Write 0 to GPPUDCLK0 to make it take effect.
  mmio_write(GPPUDCLK0, 0x00000000);

  // Clear pending interrupts.
  mmio_write(UART0_ICR, 0x7FF);

  // Set integer and fractional part of baurd rate.
  // Divider = UART_CLOICK/(16 * Baud)
  // Fraction part register = (Fractional part * 64) + 0.5
  // UART_CLOCK = 3000000; Baud = 115200.

  // Divider = 3000000 / (16 * 115200) = 1.627 = ~1.
  // Fractional part register = (.627 * 64) + 0.5 = 40.6 = ~40
  mmio_write(UART0_IBRD, 1);
  mmio_write(UART0_FBRD, 40);

  // Enable FIFO and 8 bit data transmission (1 stop bit, no partiy).
  mmio_write(UART0_LCRH, (1 << 4) | (1 << 5) | (1 << 6));

  // Mask all interrupts.
  mmio_write(UART0_IMSC, (1 << 1) | (1 << 4) | (1 << 5) | (1 << 6) |
	     (1 << 7) | (1 << 8) | (1 << 9) | (1 << 10));

  // Enable UART0, receive and transfer part of UART.
  mmio_write(UART0_CR, (1 << 0) | (1 << 8) | (1 << 9));
}


void uart_putc(unsigned char byte) {
  // Wait for UART to become ready to transmit.
  while (mmio_read(UART0_FR) & (1 << 5)) {

  }
  mmio_write(UART0_DR, byte);
}


unsigned char uart_getc() {
  // Wait for UART to have received something.
  while (mmio_read(UART0_FR) & (1 << 4)) {

  }
  return mmio_read(UART0_DR);
}


void uart_write(const unsigned char* buffer, size_t size) {
  for (size_t i = 0; i < size; i++) {
    uart_putc(buffer[i]);
  }
}


void uart_puts(const char* str) {
  uart_write((const unsigned char*) str, strlen(str));
}

#if defined(__cplusplus)
extern "C" // Use C linkage for kernal main.
#endif
void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags) {
  (void) r0;
  (void) r1;
  (void) atags;

  uart_init();
  uart_puts("Hello, kernel World!\r\n");

  while (true) {
    uart_putc(uart_getc());
  }
}
