#include "vcio.h"
#include "../mmio.h"

#define M_BOX_READ   0x00
#define M_BOX_POLL   0x10
#define M_BOX_SENDER 0x14
#define M_BOX_STATUS 0x18
#define M_BOX_CONFIG 0x1c
#define M_BOX_WRITE  0x20


static uint32_t getMailboxStatus ();


int MailboxWrite (uint8_t channel, uint32_t data28) {
    // Return an error if we have an invalid channel
    if (channel > 7) {
        return -1;
    }
    
    // Return an error if the bottom 4 bits of \a data28 are non-zero
    if (data28 & 0xf) {
        return -1;
    }
    
    // Wait until the mailbox is not full
    while (getMailboxStatus () & M_BOX_FULL) {
        // TRAP!!!
    }
    
    mmio_write (M_BOX_BASE + M_BOX_WRITE, data28 | channel);
    return 0;
}


int MailboxRead (uint8_t channel) {
    volatile int mail;
    
    // Return an error if we have an invalid channel
    if (channel > 7) {
        return -1;
    }
    
    while (getMailboxStatus () & M_BOX_EMPTY) {
        // Trap here until the mailbox has a message
    }
    
    mail = mmio_read (M_BOX_BASE + M_BOX_READ);
    
    // Return an error if we read from the wrong channel
    if ((mail & 0xf) != channel) {
        return -1;
    }
    
    // Return top 28 bits
    return mail >> 4;
}


static uint32_t getMailboxStatus() {
  return mmio_read ((uint32_t) M_BOX_BASE + (uint32_t) M_BOX_STATUS);
}
