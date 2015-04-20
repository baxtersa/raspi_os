#include "vcio.h"

#define M_BOX_FULL 0x80000000
#define M_BOX_EMPTY 0x40000000

#define M_BOX_READ   0x00
#define M_BOX_STATUS 0x18
#define M_BOX_WRITE  0x20


static uint32_t getMailboxStatus();


int MailboxRead(uint8_t channel, uint32_t* pData28) {
  // Return an error if we have an invalid channel
  if (channel > 7) {
    return -1;
  }

  // Loop until something is received on the channel
  while (1) {
    while (getMailboxStatus() & M_BOX_EMPTY) {
      // Trap here until the mailbox has a message
    }

    pData28 = (uint32_t*) (M_BOX_BASE + M_BOX_READ);

    if ((*pData28 & 0xF) == channel) {
      *pData28 =  *pData28 >> 4;
      return 0;
    }
  }
}


int MailboxWrite(uint8_t channel, uint32_t data28) {
  // Return an error if we have an invalid channel
  if (channel > 7) {
    return -1;
  }

  // Wait until the mailbox is not full
  while (getMailboxStatus() & M_BOX_FULL) {
    // TRAP!!!
  }

  *(uint32_t*)M_BOX_WRITE = data28 | channel;
  return 0;
}


static uint32_t getMailboxStatus() {
  return * (uint32_t*) (M_BOX_BASE + M_BOX_STATUS);
}
